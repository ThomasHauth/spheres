#pragma once

#include <boost/noncopyable.hpp>

/**
 * Class which can be used to implement a tick-like logic
 * which happens every fixed amount of time
 */
class TickAction: boost::noncopyable {
public:
	/**
	 * Create a new tick action class
	 * @param tick tick in seconds
	 */
	TickAction(float tick = 1.0f) :
			m_tick(tick), m_sumTick(0.0f) {

	}

	/**
	 * if returned true, the time interval given with "tick" has been crossed
	 * @param dt time in seconds since the last call to the validate method
	 */
	bool validate(const float dt) {
		m_sumTick += dt;
		if (m_sumTick >= m_tick) {
			m_sumTick -= m_tick;
			return true;
		}
		return false;
	}

private:
	/**
	 * The tick time which has been configured for his class
	 */
	const float m_tick;

	/**
	 * the summed up time in calls to validate
	 */
	float m_sumTick;
};
