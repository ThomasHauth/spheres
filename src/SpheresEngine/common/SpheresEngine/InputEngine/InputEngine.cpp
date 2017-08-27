#include "InputEngine.h"

void InputEngine::process() {
	std::vector < std::pair<UserId, Input> > allInputs;

	for (auto & source : m_sources) {
		if (!source->isEnabled()) {
			source->enableSource();
		}

		auto input = source->readInput();
		allInputs.insert(allInputs.end(), input.begin(), input.end());
	}

	for (auto & input : allInputs) {
		for (auto & trans : m_transformers) {
			auto inpAction = trans->transform(input.second);
			// can also be nullptr if there is no useful transform
			if (inpAction) {
				inpAction->addUser(input.first);
				OnNewInputAction.signal(inpAction.get());
				m_inputActions.push_back(std::move(inpAction));
			}
		}
	}
}
