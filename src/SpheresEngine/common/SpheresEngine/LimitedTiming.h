#pragma once

#include <boost/noncopyable.hpp>
#include <chrono>

#include <SpheresEngine/Timing.h>

namespace chr = std::chrono;

/**
 * this class ensures that at least n seconds are waited before the next execution
 * all times in seconds
 */
class LimitedTiming: public Timing {
public:

	/**
	 * Create the class and initialize with the minimumWait time
	 */
	LimitedTiming(float minimumWait) :
			m_minimumWait(minimumWait) {

	}

	/**
	 * End the game loop and wait (if necessary) until at least the minimum wait time
	 * has passed
	 */
	float endWithWait() {
		const float deltaT = end();

		const float additionalWait = m_minimumWait - deltaT;
		if (additionalWait > 0.0f) {
			wait(additionalWait);
			return m_minimumWait;
		} else {
			return deltaT;
		}
	}

	/**
	 * Get the amount of minium wait which has been setup with this class
	 */
	float getMinimumWait() const {
		return m_minimumWait;
	}

private:
	/**
	 * the minimum time which needs to be waited before the execution
	 * is continued
	 */
	float m_minimumWait = 0.01f;
};

