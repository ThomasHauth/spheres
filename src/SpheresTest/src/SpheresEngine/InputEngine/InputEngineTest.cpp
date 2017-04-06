#include <SpheresEngine/InputEngine/InputEngine.h>
#include <SpheresEngine/Util.h>

#include <gtest/gtest.h>

#include <utility>

/*
 *
 todo: how do we know that none of the used entities has been destroyed ?
 can Animations create new entities ?!?! should do
 */

class TestInputAction: public InputAction {
public:

};

class TestInputTransformer: public InputTransformer {
public:
	std::unique_ptr<InputAction> transform(Input) override {
		return std14::make_unique<TestInputAction>();
	}
};

class TestSource: public InputSource {
public:
	virtual ~TestSource() = default;

	std::vector<std::pair<UserId, Input> > readInput() override {

		Input ip = Input::createDirection(Input::Device::Keyboard,
				Vector2(1.0, -1.0));

		return {std::make_pair( UserId(), ip )};

	}

};

/*
 Will never return an input action
 */
class NullInputTransformer: public InputTransformer {
public:
	virtual ~NullInputTransformer() = default;

	std::unique_ptr<InputAction> transform(Input) override {
		return nullptr;
	}
};

TEST(InputEngineTest, transformInput) {
	InputEngine ie;

	ie.addSource(std14::make_unique<TestSource>());
	ie.addTransformer(std14::make_unique<TestInputTransformer>());

	ASSERT_EQ(size_t(0), ie.getInputActions().size());

	ie.process();

	ASSERT_EQ(size_t(1), ie.getInputActions().size());
	ie.clearInputActions();
	ASSERT_EQ(size_t(0), ie.getInputActions().size());

	// collect multiple inputs
	ie.process();
	ie.process();
	ASSERT_EQ(size_t(2), ie.getInputActions().size());
}

TEST(InputEngineTest, transformInputNullTransformer) {
	InputEngine ie;

	ie.addSource(std14::make_unique<TestSource>());
	ie.addTransformer(std14::make_unique<NullInputTransformer>());

	ASSERT_EQ(size_t(0), ie.getInputActions().size());

	ie.process();
	ASSERT_EQ(size_t(0), ie.getInputActions().size());
}

