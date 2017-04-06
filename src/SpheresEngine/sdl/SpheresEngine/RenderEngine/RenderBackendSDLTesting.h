#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>

/**
 * SDL Backend implementation for unit testing purposes. Sets up
 * SDL OpenGL so shaders can be test compiled.
 */
class RenderBackendSDLTesting {
public:

	/**
	 * Open an SDL display
	 */
	void openDisplay();

	/**
	 * Close the SDL display
	 */
	void closeDisplay();

	/**
	 * Calls glewInit
	 */
	void initRenderer();

	/**
	 * Nothing done atm.
	 */
	void closeRenderer() {

	}

	/**
	 * Swap displays
	 */
	void present();

	/**
	 * Returns true if the Backend has been correctly initialized
	 */
	bool isValid() const {
		return !m_fail;
	}

private:
	/**
	 * Pointer to the created display window
	 */
	SDL_Window* m_displayWindow = nullptr;

	/**
	 * Initialized GL Context
	 */
	SDL_GLContext m_glContext;

	/**
	 * Is true, if the backend has been correctly initialized
	 */
	bool m_fail = false;
};
