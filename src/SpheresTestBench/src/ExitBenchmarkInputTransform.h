#pragma once

#include <SpheresEngine/InputEngine/InputTransformer.h>

/**
 * Generic Input actions for benchmark purposes
 */
class BenchmarkInputAction: public InputAction {
public:

	/**
	 * Action type which exist, only the exit action is currently relevant
	 */
	enum class ActionType {
		NotSet, Terminate
	};

	/**
	 * Create a new input action class
	 */
	BenchmarkInputAction(ActionType action) :
			Action(action) {

	}

	/**
	 * support proper deallocation
	 */
	virtual ~BenchmarkInputAction() = default;

	/**
	 * Stores the actual action type
	 */
	ActionType Action = ActionType::NotSet;
};

/**
 * This transformation checks all input and creates a Terminate action
 * if the user pressed the Escape button.
 */
class ExitBenchmarkInputTransform: public InputTransformer {
public:

	/**
	 * support proper class cleanup
	 */
	virtual ~ExitBenchmarkInputTransform() = default;

	/**
	 * Transform pressing of esc-button to an terminate action
	 */
	std::unique_ptr<InputAction> transform(Input ip) override {
		if (ip.type() == Input::Type::ButtonDown) {
			if (ip.buttonType() == Input::ButtonType::Escape) {
				// still hardwired, will be done by an aspect later
				return std14::make_unique<BenchmarkInputAction>(
						BenchmarkInputAction::ActionType::Terminate);
			}
		}

		// nothing to report
		return nullptr;
	}

};

