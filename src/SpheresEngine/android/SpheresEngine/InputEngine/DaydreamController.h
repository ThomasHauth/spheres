#pragma once

#include <SpheresEngine/InputEngine/InputSource.h>

#include <vr/gvr/capi/include/gvr.h>
#include <vr/gvr/capi/include/gvr_types.h>
#include <vr/gvr/capi/include/gvr_controller.h>

#include <vector>

/**
 * Implementation of the Google VR daydream controller
 */
class DaydreamControllerSource: public InputSource {
public:

	/**
	 * Create the controller source. Needs a pointer to the controller api
	 */
	DaydreamControllerSource(gvr::ControllerApi * controller_api) :
			m_controller_api(controller_api) {

	}

	/**
	 * Will return SDL Keyboard and mouse input
	 */
	std::vector<UserInputPair> readInput() override;

	/**
	 * Queries the GVR for new controller input. This needs to be called
	 * every game frame
	 */
	void updateControllerState();

private:
	/**
	 * All inputs which have been collected during the last loop
	 */
	std::vector<UserInputPair> m_inputs;

	/**
	 * Pointer to GVR controller API
	 */
	gvr::ControllerApi * m_controller_api = nullptr;

	/**
	 * most recently queried controller state
	 */
	gvr::ControllerState m_controller_state;

};
