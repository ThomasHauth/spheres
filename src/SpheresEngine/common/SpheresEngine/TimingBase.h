#pragma once

#include <boost/noncopyable.hpp>
#include <chrono>

namespace chr = std::chrono;

/**
 * Base class for measurng timing differences
 */
class TimingBase: boost::noncopyable {
public:
	/**
	 * Create and start the time taking
	 */
	TimingBase() {
		start();
	}

	/**
	 * Take the start time
	 */
	void start() {
		m_startTime = chr::high_resolution_clock::now();
	}

	/**
	 * return the time difference in seconds between the calls to start() and
	 * end()
	 */
	float delta() const {
		std::chrono::milliseconds secs = chr::duration_cast<
				std::chrono::milliseconds>(m_endTime - m_startTime);
		return secs.count() * 0.001f;
	}

	/**
	 * Take the stop time
	 */
	float end() {
		m_endTime = chr::high_resolution_clock::now();
		return delta();
	}

private:
	/**
	 * Time point when start() was called
	 */
	chr::high_resolution_clock::time_point m_startTime;

	/**
	 * Time point when end() was called
	 */
	chr::high_resolution_clock::time_point m_endTime;
};

