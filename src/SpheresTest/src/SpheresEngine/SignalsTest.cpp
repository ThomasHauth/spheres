#include <gtest/gtest.h>

#include <SpheresEngine/Signals.h>

TEST(Signals, subscribe_unsubscibe) {

	slots::Slot<int> testSlot;

	int callCount = 0;

	auto sub1id = testSlot.subscribe([&callCount](int) {callCount++;});
	auto sub2id = testSlot.subscribe([&callCount](int) {callCount++;});

	testSlot.signal(23);

	ASSERT_EQ(2, callCount);

	// remove one
	testSlot.unsubscribe( sub1id);

	testSlot.signal(23);

	ASSERT_EQ(3, callCount);

	// remove the other one
	testSlot.unsubscribe( sub2id);
}

