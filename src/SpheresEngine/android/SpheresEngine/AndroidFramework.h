#pragma once

#include <SpheresEngine/RenderEngine/RenderBackendAndroid.h>
#include <SpheresEngine/JavaInterface.h>
#include <SpheresEngine/Engines.h>
#include <SpheresEngine/ThreadedGameLoop.h>
#include <SpheresEngine/AnimationEngine/AnimationEngine.h>
#include <SpheresEngine/RenderEngine/RenderEngine.h>
#include <SpheresEngine/EntityEngine/EntityEngine.h>
#include <SpheresEngine/EntityEngine/CommonEntities/PositionedEntity.h>
#include <SpheresEngine/EntityEngine/CommonEntities/CameraEntity.h>
#include <SpheresEngine/Physics/PhysicsEngine.h>
#include <SpheresEngine/InputEngine/InputEngine.h>
#include <SpheresEngine/ResourceEngine/ResourceEngineAbstract.h>
#include <SpheresEngine/RenderEngine/Targets/CameraTarget.h>
#include <SpheresEngine/RenderEngine/CommonOpenGL/MeshRenderer.h>
#include <SpheresEngine/RenderEngine/CommonOpenGL/ParticlesRenderer.h>

#include <memory>

/**
 * Base class for all game frameworks running on the android platform
 */
template<class TRenderBackend>
class AndroidFramework {
public:
	/**
	 * Provide virtual dtor for proper deallocation in inherited classes
	 */
	virtual ~AndroidFramework() = default;

	/**
	 * Update the java interface in the engine code. This method needs
	 * to be called before any other calls are done on spheres engine code.
	 */
	void setJavaInterface(JNIEnv * jnienv, jobject activity) {
		m_javaInterface = std14::make_unique<JavaInterfaceNonNative>(jnienv,
				activity);
		m_resources.setJavaInterface(m_javaInterface.get());
		// todo
		//getSoundEngine().setJavaInterface(m_javaInterface.get());
	}

	/**
	 * Frees all loaded textures
	 */
	void freeAllTextures() {
		m_resources.freeAllTextures();
	}

	/**
	 * Render backend is created ouside of this common loop, because most of the time it needs
	 * specific settings from the context etc.
	 */
	void setRenderBackend(std::unique_ptr<TRenderBackend> rb) {
		m_renderBackend = std::move(rb);
	}

	/**
	 * Initialize the game loop
	 */
	void initGameLoop() {
		// only check, if the render engine is not initialized yet
		if (!m_renderBackend.get() && !m_re.get()) {
			logging::Fatal()
					<< "RenderBackend needs to be created before the GameLoop is initialized";
			return;
		}

		m_re = std14::make_unique<RenderEngine>(std::move(m_renderBackend),
				m_resources);

		m_gameLoop = std14::make_unique<ThreadedGameLoop>(*m_re.get(),
				m_entityEngine, m_animationEngine, m_inputEngine,
				m_physicsEngine);

		m_lmdLogic = m_gameLoop->getLogicLambda();
		m_lmdRender = m_gameLoop->getRenderLambda();
	}

	/**
	 * Initialize the render engine
	 */
	void initRenderEngine(AndroidInitData initData = AndroidInitData()) {
		if (!m_gameLoop) {
			logging::Fatal()
					<< "GameLoop needs to be initialized before initRenderEngine()";
		}
		auto lmdInitRenderEngine = m_gameLoop->getRenderInitLambda();
		auto lmdAddRenderer = m_gameLoop->getAddRendererLambda();

		lmdInitRenderEngine();
		lmdAddRenderer();
	}

	/**
	 * this needs to be overwritten by any implementation
	 * to setup the content of the application
	 */
	virtual void initContent() {

		// just here for testing
		auto mv1 = new MeshVisual("debug_box", "debug_texture");
		mv1->getData().Center = glm::vec3(0, 0, 0.5);
		auto prepId = m_re->addToPrepareVisual(mv1);

		auto boxEntity = std14::make_unique<PositionedEntity>();
		auto boxEntityPtr = boxEntity.get();
		boxEntity->addVisualPlaceholder(prepId);
		m_entityEngine.addEntity(std::move(boxEntity));

	}

	/**
	 * Execute next step in the game loop
	 * @param timeDelta time since last call in seconds
	 */
	void step(float timeDelta) {
		//m_gameLoop->step(timeDelta);
		if (m_first) {

			//m_re->render();
			m_first = false;
		}

		// todo: currently in one thread, can be in 2 threads later
		queryInput();
		m_lmdLogic(timeDelta);
		m_lmdRender(timeDelta);
		if ((stepCount % 100) == 0)
			logging::Info() << "Executed step " << stepCount;
		stepCount++;
	}

	/**
	 * Can be overwritten to handle input from the android side
	 */
	virtual void queryInput() = 0;

	/**
	 * Resume game
	 */
	void resumeGame() {
		resumeInput();
	}

	/**
	 * Pause input
	 */
	void pauseGame() {
		pauseInput();
	}

	/**
	 * Override this to implement a pausing to the input
	 */
	virtual void pauseInput() {

	}

	/**
	 * Override this to implement a resuming to the input
	 */
	virtual void resumeInput() {

	}

	Engines getEngines() {
		return Engines( m_entityEngine, m_inputEngine, *m_re.get(), m_animationEngine );
	}

protected:
	/**
	 * Stores how often the step method has been called
	 */
	size_t stepCount = 0;

	/**
	 * Java interface class used for native C++ calls to Java
	 * by various backends of the spheres engine
	 */
	own<JavaInterfaceNonNative> m_javaInterface;

	/**
	 * Render Engine instancce
	 */
	own<RenderEngine> m_re;

	/**
	 * Instance of the game loop class
	 */
	own<ThreadedGameLoop> m_gameLoop;

	/**
	 * Lambda function used for logic work
	 */
	std::function<void(float)> m_lmdLogic;

	/**
	 * Lambda function used for render work
	 */
	std::function<void(float)> m_lmdRender;

	/**
	 * Instance of Android Resource Engine
	 */
	ResourceEngine m_resources;

	/**
	 * Instance of Animation Engine
	 */
	AnimationEngine m_animationEngine;

	/**
	 * Instance of Entity Engine
	 */
	EntityEngine m_entityEngine;

	/**
	 * Instance of Physics engine
	 */
	PhysicsEngine m_physicsEngine;

	/**
	 * Instance of android-specific Input Engine
	 */
	InputEngine m_inputEngine;

	/**
	 * Is true if the step() method has been called the first time
	 */
	bool m_first = true;

	/**
	 * Pointer to the created RenderBackend. Might be a regular OpenGL backend
	 * or the VR render backend on Android
	 */
	TRenderBackend * m_renderBackendPtr = nullptr;

	/**
	 * Owning pointer of the Render Backend
	 * important: as soon as the game loop is initialized, this ptr gets moved into the render
	 * engine and cannot be used any more
	 */
	std::unique_ptr<TRenderBackend> m_renderBackend;
};
