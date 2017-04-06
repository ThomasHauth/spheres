
#include "AndroidVRFramework.h"

#include <SpheresEngine/RenderEngine/Targets/VREyeTarget.h>

void AndroidVRFramework::createVRRenderBackend() {
	// this will just create the render backend, but not the render engine ...
	logging::Info () << "Creating RenderBackendAndroidVR";
	auto rb =std14::make_unique<RenderBackendAndroidVR>(m_resources);
	rb->setGvrObjects(m_gvr_api.get()/*, &m_scratch_viewport*/);
	setRenderBackend(std::move(rb));
	m_renderInitialized = true;
}

void AndroidVRFramework::initAndroidInput () {
	logging::Info() << "Initializing ControllerApi";
	m_controller_api.reset(new gvr::ControllerApi);

	m_controller_api->Init(gvr::ControllerApi::DefaultOptions(),
								m_gvr_context);
	m_controller_api->Resume();
}

void AndroidVRFramework::pauseInput() {
        if (m_controller_api)
    m_controller_api->Resume();
}

void AndroidVRFramework::resumeInput() {
    if (m_controller_api)
    m_controller_api->Pause();
}

void AndroidVRFramework::ensureGameInit() {
	if (!m_renderInitialized) {
		logging::Info () << "RenderBackend needs to be initialzed before game loop, returning";
		return;
	}

	if (!b_gameInitialized) {
		logging::Info() << "Initializing Game ";

		initGameLoop();
		initAndroidInput();
		initRenderEngine();

        // create and add the Daydream controller to the input system
        m_daydream_controller = static_cast<DaydreamControllerSource*>(
        m_inputEngine.addSource(std14::make_unique<DaydreamControllerSource>(m_controller_api.get())));

		// this is specific RenderEngine setup for VR
		std::unique_ptr<RenderTargetBase> leftEyeVr = std14::make_unique<VREyeTarget>(GVR_LEFT_EYE);
		m_re->addTarget(std::move(leftEyeVr));
		std::unique_ptr<RenderTargetBase>  rightEyeVr = std14::make_unique<VREyeTarget>(GVR_RIGHT_EYE);
		m_re->addTarget(std::move(rightEyeVr));

		// todo: put at a more generic place
		m_re->getRenderBackend()->getShaderBackend().addProgramDefinition(
				"default", { { { "vtx", Shader::Type::Vertex }, { "fs",
				Shader::Type::Fragment } } });
        initContent();
		b_gameInitialized = true;

		logging::Info() << "Game initialized";
	}
}

void AndroidVRFramework::queryInput() {
    if ( m_daydream_controller ) {
        m_daydream_controller->updateControllerState();
    }

}

void AndroidVRFramework::drawFrame() {
	ensureGameInit();
	/* todo: time */

	if (m_renderInitialized) {
        auto lastDelta = m_timing.end();
        // start a new round of taking the time
        m_timing.start();
		step(lastDelta);
	} else {
		logging::Info () << "Renderer not initialized, skipping draw call";
	}
}


