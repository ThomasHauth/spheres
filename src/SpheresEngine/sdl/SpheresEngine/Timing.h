#pragma once

#include <SpheresEngine/TimingBase.h>
#include <SDL2/SDL_timer.h>

/**
 * Timing implementation for SDL
 */
class Timing: public TimingBase {
public:

	/**
	 * Implement the wait method via SDL's Delay method
	 * @param waitSeconds time to wait in seconds
	 */
	void wait(float waitSeconds) {
		// SDL_Delay  with take the time in ms
		float waitMS = waitSeconds * 1000.0f;
		SDL_Delay(round(waitMS));
	}

};
