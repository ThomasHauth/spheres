#pragma once

#include <SpheresEngine/InputEngine/InputSource.h>

#include <SDL2/SDL.h>

#include <vector>

/**
 * Input source used for SDL implementation. The input source
 * does not query SDL itself but gets forwarded the input events
 * from the game loop.
 */
class SdlSource: public InputSource {
public:

	SdlSource(bool grabMouse = true) :
			m_grabMouse(grabMouse) {
	}

	/**
	 * Implement virtal dtor for correct cleanup of class
	 */
	virtual ~SdlSource() = default;

	/**
	 * Will return SDL Keyboard and mouse input
	 */
	std::vector<UserInputPair> readInput() override;

	/**
	 * This method is called by the game loop to pass on SDL events
	 * to this input source.
	 */
	void notifyEvent(SDL_Event evt);

	void enableSource() override;

private:

	/** List of input events that have been collected during
	 * the last input round.
	 */
	std::vector<UserInputPair> m_inputs;

	bool m_grabMouse;
};
