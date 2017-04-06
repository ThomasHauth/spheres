#include "SdlSource.h"

#include <SpheresEngine/Log.h>

std::vector<InputSource::UserInputPair> SdlSource::readInput() {
	auto tmpInput = m_inputs;
	m_inputs.clear();
	return tmpInput;
}

void SdlSource::notifyEvent(SDL_Event evt) {
	Input::ButtonType btnType = Input::ButtonType::Unknown;
	if (evt.key.keysym.sym == SDLK_ESCAPE)
		btnType = Input::ButtonType::Escape;
	if (evt.key.keysym.sym == SDLK_LEFT)
		btnType = Input::ButtonType::Left;
	if (evt.key.keysym.sym == SDLK_RIGHT)
		btnType = Input::ButtonType::Right;
	if (evt.key.keysym.sym == SDLK_UP)
		btnType = Input::ButtonType::Up;
	if (evt.key.keysym.sym == SDLK_DOWN)
		btnType = Input::ButtonType::Down;

	switch (evt.type) {
	case SDL_KEYDOWN:
		m_inputs.push_back(
				std::make_pair(UserId::useDefault(),
						Input::createButtonDown(Input::Device::Keyboard,
								btnType)));
		break;
	case SDL_KEYUP:
		m_inputs.push_back(
				std::make_pair(UserId::useDefault(),
						Input::createButtonUp(Input::Device::Keyboard,
								btnType)));
		break;
	}
}
