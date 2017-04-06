#pragma once

#include <boost/noncopyable.hpp>

#include <SpheresEngine/Log.h>

/**
 * helper class to manage game actions which can only happen a certain amount / time and needs
 * some time to cooldown before being activated again.
 */
class TimeSliceAction: boost::noncopyable {
public:

	/**
	 * Active the action
	 * @param duration specifes how long the action will be active
	 */
	void activate(const float duration) {
		m_isActive = true;
		m_wasProcessed = false;
		m_timeLeft = duration;
	}

	/**
	 * continues the action and checks if the action and it's cooldown time is complete
	 * @param deltaT time in seconds since the last call to this method
	 * @param cooldown the time needed for the action to cooldown and become available again
	 */
	void validate(const float deltaT, const float cooldown = 0.0f) {
		if (m_isActive) {
			m_timeLeft -= deltaT;
			if (m_timeLeft < 0.0f) {
				reset();
				m_justDisabledCheck = true;
				if (cooldown > 0.0f)
					m_cooldown = cooldown;
			}
		}

		if (m_cooldown > 0.0f) {
			m_cooldown -= deltaT;
		}
	}

	/**
	 * Returns true if the action still being executed
	 */
	bool isActive() const {
		return m_isActive;
	}

	/**
	 * Returns true if the action has been completed since the last call to this method.
	 * If this method is called a second time, it returns false.
	 */
	bool wasJustDisabled() {
		if (m_justDisabledCheck) {
			m_justDisabledCheck = false;
			return true;
		}
		return false;
	}

	/**
	 * Returns true if the action has been processed. This value can be freely set by
	 * setProcessed(), for example by the animation engine to store that the animation
	 * does not need to be started again
	 */
	bool wasProcessed() const {
		return m_wasProcessed;
	}

	/**
	 * store that the action has been processed by an external system, for example the
	 * animation engine
	 */
	void setProcessed() {
		m_wasProcessed = true;
	}

	/** Returns true if the action can be activated again, so it is not running and
	 * the cooldown period is over.
	 */
	bool canActivate() const {
		return (!isActive() && (m_cooldown <= 0.0f));
	}
private:

	/**
	 * set the state of the class to the state before starting an action. Except for the
	 * cooldown time, which might still be running.
	 */
	void reset() {
		m_isActive = false;
		m_wasProcessed = false;
		m_timeLeft = 0.0f;
	}

	/**
	 * The time left (in seconds) this action will run
	 */
	float m_timeLeft = 0.0f;

	/**
	 * The time (in seconds) this action will need to cooldown, so to be activated again
	 */
	float m_cooldown = -1.0f;

	/**
	 * Stores if the action is currently active
	 */
	bool m_isActive = false;

	/**
	 * Stores whether the action has been completed since the last call to wasJustDisabled
	 */
	bool m_justDisabledCheck = false;

	/** will be set to true, once the action has been proceesed, like the result of a
	 * kick has been appplied to the game state
	 * this does not mean that the action is over yet, the animation etc. is still running
	 */
	bool m_wasProcessed = false;
};

