#include <SpheresEngine/AnimationEngine/AnimationEngine.h>
#include <SpheresEngine/EntityEngine/EntityEngine.h>
#include <SpheresEngine/RenderEngine/CommonOpenGL/MeshRenderer.h>
#include <SpheresEngine/RenderEngine/CommonOpenGL/ParticlesRenderer.h>
#include <SpheresEngine/RenderEngine/Targets/CameraTarget.h>

#include <SpheresEngine/ThreadedGameLoop.h>
#include <SpheresEngine/InputEngine/InputEngine.h>
#include <SpheresEngine/Pathfinding/Node.h>
#include <SpheresEngine/PhysicsEngine/PhysicsEngine.h>
#include <SpheresEngine/Performance/SectionTimer.h>
#include <SpheresEngine/LimitedTiming.h>
#include <SpheresEngine/Timing.h>

#include <atomic>

#ifdef USE_SDL
#include <SDL2/SDL.h>
#include <SpheresEngine/InputEngine/SdlSource.h>
#endif

std::function<void(float)> ThreadedGameLoop::getLogicLambda() {

	auto lmdLogic = [this] (float timeDelta) {

		// todo: might be better to keep this datastructer at a place
		// where is not always allocated fresh (right now on stack)
			VisualDataExtractContainer exCont;

#ifdef USE_SDL
			SDL_Event event;
			while (SDL_PollEvent(&event)) {
				if ( m_sdlInputSource ) {
					// forward all events to the InputSource for the Input system
					m_sdlInputSource->notifyEvent( event );
				}
			}
#endif

			{
				SectionTimer tm(GlobalTimingRepo::Rep, "input");
				m_inputEngine.process();
			}

			{
				SectionTimer tm(GlobalTimingRepo::Rep, "entities");
				m_entityEngine .step(timeDelta);
			}

			// if the aspects or entity engine did not use recent actions,
			// they will now be cleared to not build up
			m_inputEngine.clearInputActions();

			{
				SectionTimer tm(GlobalTimingRepo::Rep, "animation");
				m_animationEngine .step(timeDelta);
			}

			{
				SectionTimer tm(GlobalTimingRepo::Rep, "physics");
				m_physics.step(timeDelta);
			}

			{
				SectionTimer tm(GlobalTimingRepo::Rep, "extract_visual");

				{
					std::lock_guard<std::mutex> lg ( m_preparedVisualsAccess);
					//std::cout << " Got " << m_preparedVisuals.size() <<" vis" << std::endl;
					this->m_entityEngine.updatePreparedVisuals(m_preparedVisuals);
					m_preparedVisuals.clear();
				}
			}
			// update the visual which might have change in one of the last render rounds
			{
				std::lock_guard<std::mutex> lg ( m_rendererVisualChangesAccess);
				this->m_entityEngine.updateVisuals(m_rendererVisualChanges);
				m_rendererVisualChanges.clear();
			}

			this->m_entityEngine.extractVisualData(exCont);

			// update central instance
			{
				std::lock_guard<std::mutex> lg (m_extractContainerAccess);
				m_extractContainer = exCont;
			}
		};

	return lmdLogic;
}

std::function<void()> ThreadedGameLoop::getRenderInitLambda() {
	return [this] () {
		logging::Info() << "ThreadedGameLoop - Opening Display";
		m_renderEngine.openDisplay();
		logging::Info() << "ThreadedGameLoop - Initializing Renderer";
		m_renderEngine.initRenderer();

		logging::Info() << "ThreadedGameLoop - OpenGL Subsystem initialized";
	};
}

std::function<void()> ThreadedGameLoop::getAddRendererLambda() {
	return [this] () {
		logging::Info() << "ThreadedGameLoop - Adding default Renderer";
		m_renderEngine.addRenderer(std14::make_unique<MeshRenderer>());
		m_renderEngine.addRenderer(std14::make_unique<ParticlesRenderer>());

		logging::Info() << "ThreadedGameLoop - Added default Renderer";
	};
}

std::function<void(float)> ThreadedGameLoop::getRenderLambda() {

	return [this]( float /*deltaTime*/) {
		{
			SectionTimer tm(GlobalTimingRepo::Rep, "render::update_visual_data");
			VisualDataExtractContainer exCont;
			{
				std::lock_guard<std::mutex> lg (m_extractContainerAccess);
				// should not take too long, as it is just creating a copy
				m_renderEngine.updateVisualData(m_extractContainer);
			}
		}

		{
			SectionTimer tm(GlobalTimingRepo::Rep, "render::render");
			m_renderEngine.render();

			{
				std::lock_guard<std::mutex> lg ( m_preparedVisualsAccess);
				auto visThisRound = m_renderEngine.popPreparedVisuals();

				//std::cout << " visThisRound" << visThisRound.size() << std::endl;
				// only add here and never overwrite, we do not if/when the logic
				// thread had time to access
				m_preparedVisuals.insert(m_preparedVisuals.end(), visThisRound.begin(), visThisRound.end());
			}
			{
				std::lock_guard<std::mutex> lg ( m_rendererVisualChangesAccess);
				auto visChange = m_renderEngine.popVisualChange();

				//std::cout << " visThisRound" << visThisRound.size() << std::endl;
				// only add here and never overwrite, we do not if/when the logic
				// thread had time to access
				m_rendererVisualChanges .insert(m_rendererVisualChanges.end(), visChange.begin(), visChange.end());
			}
		}
	};
}

void ThreadedGameLoop::run() {
	logging::Debug() << "ThreadedGameLoop::run";

	bool quitApp = false;
	Timing loopTiming;

	if (m_profile) {
		GlobalTimingRepo::Rep.clearFile(m_profileFileName);
	}

	logging::Info() << "Starting game loop";

	auto lmdLogicThread = [this] () {
		logging::Info() << "Started logic thread";
		LimitedTiming timing(1.0f/m_maxGameloopRate);

		auto lmdLogic = getLogicLambda();
		size_t count = 0;

		m_terminate = false;
		float timeDelta = timing.getMinimumWait( );
		while (!m_terminate) {
			timing.start();
			// todo: do not compute the logic loop too often, otherwise the rendering thread
			// might lag behind
			lmdLogic(timeDelta);
			// is the sdl available, if yes, check for events...

			timeDelta = timing.endWithWait();
			count++;

			if (m_exitAfterThreadIterations.isValid()) {
				if (count >= m_exitAfterThreadIterations.get() ) {
					logging::Info() << "Leaving logic thread because " << count << " done";
					return;
				}
			}
		}

		logging::Info() << "Leaving logic thread";
	};

	auto lmdRenderThread = [this] {
		logging::Info() << "Started render thread";
		LimitedTiming timing(1.0f/m_maxFramerate);

		// needs to be executed in the render thread, due to
		// OpenGL's (non-existing) threading model
			auto lmdRenderInit = getRenderInitLambda();
			auto lmdAddRenderer = getAddRendererLambda();
			auto lmdRenderLambda = getRenderLambda();
			lmdRenderInit();
		// add the various renderers
			lmdAddRenderer();
			size_t count = 0;

		// adding the camera target is only useful, if we are in a non-VR environmen
		// VR uses VREyeTargets !
			auto cameraTarget = std14::make_unique<CameraTarget>();
			if (m_screenshotFilename.isValid()) {
				cameraTarget->enableStoreScreenshot ( m_screenshotFilename.get() );
			}
			m_renderEngine.addTarget(std::move(cameraTarget));

			float timeDelta = timing.getMinimumWait( );
			while (!m_terminate) {
				timing.start();

				lmdRenderLambda( timeDelta);

				timeDelta = timing.endWithWait();
				count++;

				if (m_exitAfterThreadIterations.isValid()) {
					if (count >= m_exitAfterThreadIterations.get() ) {
						logging::Info() << "Leaving render thread because " << count << " done";
						return;
					}
				}
			}
			logging::Info() << "Leaving render thread";
		};

	std::thread logicThread(lmdLogicThread);
	std::thread renderThread(lmdRenderThread);

	logicThread.join();
	renderThread.join();
}

