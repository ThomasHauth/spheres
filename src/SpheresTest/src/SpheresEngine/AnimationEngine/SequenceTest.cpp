#include <SpheresEngine/AnimationEngine/Sequence.h>

#include <gtest/gtest.h>

TEST(SequenceTest, linearSequence) {

	const float duration = 10.0f;

	float test_duration;
	float test_current;
	size_t call_count = 0;

	auto testLambda = SequenceModifier::buildLinear([&](float cur, float dur)
	{
		test_duration = dur;
		test_current = cur;
		call_count++;
	});
	Sequence sq(duration, testLambda);

	ASSERT_EQ(size_t(0), call_count);

	// has been running for 6 seconds now
	sq.step(5.0f);
	ASSERT_EQ(size_t(1), call_count);
	ASSERT_FLOAT_EQ(10.0, test_duration);
	// only to seconds running so far ...
	ASSERT_FLOAT_EQ(5.0, test_current);
}
