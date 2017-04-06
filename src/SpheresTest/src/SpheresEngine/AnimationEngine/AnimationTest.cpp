#include <SpheresEngine/AnimationEngine/Animation.h>

#include <gtest/gtest.h>

/*
 *
 todo: how do we know that none of the used entities has been destroyed ?
 can Animations create new entities ?!?! should do
 */

TEST(AnimationTest, multipleSequencesInAnimation) {

	const float duration = 2.0f;
	const float abs_start = 4.0f;

	float test_duration1;
	float test_current1;
	size_t call_count1 = 0;

	float test_duration2;
	float test_current2;
	size_t call_count2 = 0;

	auto testLambda1 = SequenceModifier::buildLinear([&](float cur, float dur)
	{
		test_duration1 = dur;
		test_current1 = cur;
		call_count1++;
	});
	Sequence sq1(duration, testLambda1);

	auto testLambda2 = SequenceModifier::buildLinear([&](float cur, float dur)
	{
		test_duration2 = dur;
		test_current2 = cur;
		call_count2++;
	});
	Sequence sq2(duration, testLambda2);

	Animation anm;
	anm.addSequence(0.2, sq1);
	anm.addSequence(abs_start, sq2);

	anm.step(1.0f);

	ASSERT_EQ(size_t(1), call_count1);
	ASSERT_EQ(size_t(0), call_count2);
	ASSERT_TRUE(!anm.isRetired());

	// we give the total animation time here and no delta
	anm.step(1.0 + 3.2f);

	// sequence 1 is already complete
	ASSERT_EQ(size_t(1), call_count1);
	ASSERT_EQ(size_t(1), call_count2);
	ASSERT_TRUE(!anm.isRetired());

	anm.step(1.0f + 3.2f + 3.2f);

	// call count should not increase, because the duration of the animations is
	// over
	ASSERT_EQ(size_t(1), call_count1);
	ASSERT_EQ(size_t(1), call_count2);
	ASSERT_TRUE(anm.isRetired());
}

TEST(AnimationTest, sequenceRepeat) {

	const float duration = 1.0f;

	float test_duration1;
	float test_current1;
	size_t call_count1 = 0;

	auto testLambda1 = SequenceModifier::buildLinear([&](float cur, float dur)
	{
		test_duration1 = dur;
		test_current1 = cur;
		call_count1++;
	});
	Sequence sq1(duration, testLambda1);

	Animation anm( { }, true);
	anm.addSequence(0.2, sq1);

	anm.step(1.0f);
	ASSERT_EQ(size_t(1), call_count1);

	anm.step(1.0f);
	// should be called again, because it was set on autoRepeat
	ASSERT_EQ(size_t(2), call_count1);
}
