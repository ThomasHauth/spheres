
#include <SpheresEngine/InputEngine/DaydreamController.h>

#include <SpheresEngine/Log.h>
#include <SpheresEngine/VectorTypesGvr.h>

std::vector<InputSource::UserInputPair> DaydreamControllerSource::readInput() {
	auto tmpInput = m_inputs;
	m_inputs.clear();
	return tmpInput;
}


void DaydreamControllerSource::updateControllerState() {

    const int32_t old_status = m_controller_state.GetApiStatus();
    const int32_t old_connection_state = m_controller_state.GetConnectionState();

    // Read current controller state.
    m_controller_state.Update(*m_controller_api);

    // Print new API status and connection state, if they changed.
    if (m_controller_state.GetApiStatus() != old_status ||
        m_controller_state.GetConnectionState() != old_connection_state) {
        logging::Info () << " controller API status: " << gvr_controller_api_status_to_string(m_controller_state.GetApiStatus())
        << " connection state: " <<  gvr_controller_connection_state_to_string(
                     m_controller_state.GetConnectionState());
    }

    // transfer all the known information

    if (m_controller_state.GetTouchDown()) {
        m_inputs.push_back( std::make_pair( UserId::useDefault(), Input::createButtonDown( Input::Device::MagicWand, Input::ButtonType::Touchpad)));
        logging::Info() << "MagicWand touch down !";
    }

    if (m_controller_state.GetTouchUp()) {
        m_inputs.push_back( std::make_pair( UserId::useDefault(), Input::createButtonUp( Input::Device::MagicWand, Input::ButtonType::Touchpad)));
    }

    // always store the orientation
    auto orient = m_controller_state.GetOrientation();
    m_inputs.push_back(
            std::make_pair( UserId::useDefault(), Input::createOrientation( gvrToGlm(orient) )));
}
