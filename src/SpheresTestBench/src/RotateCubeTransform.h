#pragma once

#include <SpheresEngine/InputEngine/InputTransformer.h>
#include <SpheresEngine/VectorTypes.h>

/**
 * Action to rotate an object in the benchmark
 */
class RotateAction final: public InputAction {
public:
	/**
	 * The rotation speed is indicated by speed parameter which is [-1,1]
	 */
	RotateAction(Vector3 rotAxis, float speed = 0.0f) :
			RotationAxis(rotAxis), Speed(speed) {
	}

	/**
	 * support inherited dtor
	 */
	virtual ~RotateAction() = default;

	/**
	 * The axis to be rotated around
	 */
	Vector3 RotationAxis = Vector3::zero();

	/**
	 * The speed of the rotation
	 */
	float Speed = 0.0f;
};

/**
 * Transforms the user's key input to the RotateAction
 */
class RotateCubeTransform: public InputTransformer {
public:
	/**
	 * Support correct destruction
	 */
	virtual ~RotateCubeTransform() = default;

	/**
	 * check for button down and create an rotation action accordingly
	 */
	std::unique_ptr<InputAction> transform(Input ip) override {
		if (ip.type() == Input::Type::ButtonDown) {
			if (ip.buttonType() == Input::ButtonType::Left) {
				// still hardwired, will be done by an aspect later
				return std14::make_unique<RotateAction>(Vector3::unitY(), 1.0f);
			}
			if (ip.buttonType() == Input::ButtonType::Right) {
				// still hardwired, will be done by an aspect later
				return std14::make_unique<RotateAction>(Vector3::unitY(), -1.0f);
			}
			if (ip.buttonType() == Input::ButtonType::Up) {
				// still hardwired, will be done by an aspect later
				return std14::make_unique<RotateAction>(Vector3::unitZ(), 1.0f);
			}
			if (ip.buttonType() == Input::ButtonType::Down) {
				// still hardwired, will be done by an aspect later
				return std14::make_unique<RotateAction>(Vector3::unitZ(), -1.0f);
			}
		}
		// nothing to report
		return nullptr;
	}

};

