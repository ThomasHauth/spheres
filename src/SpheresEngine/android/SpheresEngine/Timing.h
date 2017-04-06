#pragma once

#include <SpheresEngine/TimingBase.h>

#include <unistd.h>

/**
 * Timing class implemenation for android
 */
class Timing: public TimingBase {
public:
	/**
	 * Just forward ctor to the base class
	 */
	Timing() :
			TimingBase() {
	}

	/**
	 * Wait for given amount of seconds
	 */
	static void wait(long waitSeconds) {
		sleep(waitSeconds);
	}

};

