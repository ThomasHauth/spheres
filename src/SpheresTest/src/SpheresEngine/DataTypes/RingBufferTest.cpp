#include <gtest/gtest.h>

#include <SpheresEngine/DataTypes/RingBuffer.h>

TEST(RingBufferTest, addAndRetrieveItems) {

	RingBuffer<int, 4> ringBuffer;

	// add entries, but don't fill the buffer fully
	ringBuffer.push(1);
	ringBuffer.push(2);
	ringBuffer.push(3);

	ASSERT_EQ(3, ringBuffer.get(0));
	ASSERT_EQ(2, ringBuffer.get(-1));
	ASSERT_EQ(1, ringBuffer.get(-2));
	ASSERT_EQ(1, ringBuffer.get(1));

	ASSERT_EQ(size_t(3), ringBuffer.size());
	ASSERT_EQ(true, ringBuffer.hasEntries());

	// add two new items (which will wrap the buffer)
	ringBuffer.push(4);
	ringBuffer.push(5);

	ASSERT_EQ(5, ringBuffer.get(0));
	ASSERT_EQ(4, ringBuffer.get(-1));
	ASSERT_EQ(3, ringBuffer.get(-2));
	ASSERT_EQ(5, ringBuffer.last());

	// the first entry (1) was dropped because the ring buffer
	// has reached its limit
	ASSERT_EQ(2, ringBuffer.get(1));
	ASSERT_EQ(size_t(4), ringBuffer.size());

	ringBuffer.clear();
	ASSERT_EQ(size_t(0), ringBuffer.size());
}

