#pragma once

//#include <android_native_app_glue.h>
//#include <android/asset_manager.h>
//#include <android/native_activity.h>
#include <SpheresEngine/AndroidFramework.h>

#include <SpheresEngine/RenderEngine/RenderBackendAndroidVR.h>
#include <SpheresEngine/InputEngine/DaydreamController.h>
#include <SpheresEngine/Timing.h>

#include <vr/gvr/capi/include/gvr.h>
#include <vr/gvr/capi/include/gvr_audio.h>
#include <vr/gvr/capi/include/gvr_types.h>
#include <vr/gvr/capi/include/gvr_controller.h>

#include <memory>

class InputSystem;

/**
 * Framework class which can be used to implement Google VR apps
 */
class AndroidVRFramework: public AndroidFramework<RenderBackendAndroidVR> {
public:
	/**
	 * Create the android vr framework and pass in the GVR context and audio api
	 */
	AndroidVRFramework(gvr_context* gvr_context,
			std::unique_ptr<gvr::AudioApi> gvr_audio_api) :
			m_gvr_context(gvr_context), m_gvr_api(
					gvr::GvrApi::WrapNonOwned(gvr_context)), m_gvr_audio_api(
					std::move(gvr_audio_api)), AndroidFramework<
					RenderBackendAndroidVR>() {

	}

	/**
	 * Properly de-construct in derived class
	 */
	virtual ~AndroidVRFramework() = default;

	/**
	 * Creates the AndroidVR-specific render backend, stores it and will hand it over to the
	 * RenderEngine once it gets created. This is in a dedicated method so it can be called alone
	 * because Android want's to do this in the surface create
	 */
	void createVRRenderBackend();

	/**
	 * executes one simulation step and render the result
	 */
	void drawFrame();

	/**
	 * Initializes the Google VR ControllerApi
	 */
	void initAndroidInput();

	/**
	 * Pauses the input
	 */
	void pauseInput() override;

	/**
	 * Resumes the input
	 */
	void resumeInput() override;

	/**
	 * Read input from input devices
	 */
	void queryInput() override;

private:

	/* Is called in every frame and checks whether all the Engine classes like Renderer and Resource
	 * Engine have been created, yet */
	void ensureGameInit();

	/**
	 * True if the game logic has been initialized
	 */
	bool b_gameInitialized = false;

	/**
	 * True if the render system has been initialized
	 */
	bool m_renderInitialized = false;

	/**
	 * Non-owning pointer to the GVR context
	 */
	gvr_context* m_gvr_context = nullptr;

	/**
	 * Owning pointer to the GVR api
	 */
	std::unique_ptr<gvr::GvrApi> m_gvr_api;

	/**
	 * Owning pointer to the GVR audo api
	 */
	std::unique_ptr<gvr::AudioApi> m_gvr_audio_api;

	/**
	 *  Controller API entry point
	 */
	std::unique_ptr<gvr::ControllerApi> m_controller_api;

	/**
	 * Pointer to the one daydream controller which is currently supported
	 */
	DaydreamControllerSource * m_daydream_controller = nullptr;

	Timing m_timing;
};
