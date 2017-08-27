#pragma once

#include <SpheresEngine/InputEngine/Input.h>

/**
 * Base class to implement an Input source for various input hardware (mouse, magic
 * wand etc.)
 */
class InputSource {
public:

	/**
	 * Provide virtaul dtor to support inheritance
	 */
	virtual ~InputSource() = default;

	/**
	 * Input pair whic links a user and the input he or she gave
	 */
	typedef std::pair<UserId, Input> UserInputPair;

	/**
	 * Needs to be implemented by inheriting classes to
	 * provided the input read out from the hardware
	 * @return a list of user and input pairs read out from
	 * 		   the hardware
	 */
	virtual std::vector<UserInputPair> readInput() = 0;

	/**
	 * Will be called before the first readInput call
	 * to allow the source to prepare.
	 */
	virtual void enableSource() {
		m_isEnabled = true;
	}

	/**
	 * Will be called once the source is not used any more
	 */
	virtual void disableSource() {
		m_isEnabled = false;
	}

	bool isEnabled() const {
		return m_isEnabled;
	}

protected:
	bool m_isEnabled = false;
};
