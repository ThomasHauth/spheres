#include <gtest/gtest.h>

#include <SpheresEngine/DataTypes/StaticVector.h>

TEST(StaticVectorTest, addAndIterateItems) {

	StaticVector<int> staticVector;

	staticVector.push(1);
	staticVector.push(2);
	staticVector.push(3);

	int i = 1;
	for (auto v : staticVector) {
		ASSERT_EQ(i, v);
		i++;
	}

	auto firstIt = staticVector.begin();
	staticVector.erase(firstIt);

	ASSERT_EQ(staticVector.size(), size_t(2));
	i = 1;
	for (auto v : staticVector) {
		ASSERT_EQ(i, v);
		i++;
	}

	staticVector.push(5);
	i = 1;
	for (auto v : staticVector) {
		if (i == 1) {
			ASSERT_EQ(int(5), v);
		} else {
			ASSERT_EQ(i, v);
		}
		i++;
	}
}

