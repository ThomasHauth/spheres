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
};
