#pragma once

#include <SpheresEngine/InputEngine/Input.h>
#include <SpheresEngine/InputEngine/InputAction.h>
#include <SpheresEngine/Util.h>

#include <memory>

/**
 * Base class to implement a hardware-dependent Input (for example mouse movement)
 * to abstract InputAction processed by game aspects
 */
class InputTransformer {
public:

	/**
	 * virtual dtor to support inheritance
	 */
	virtual ~InputTransformer() = default;

	/**
	 * This method needs to be overwritten by actual implementations
	 * and returns the generated input action from one input.
	 */
	virtual uniq<InputAction> transform(Input ip) = 0;

};
