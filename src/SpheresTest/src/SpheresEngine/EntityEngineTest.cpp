#include <SpheresEngine/EntityEngine/Entity.h>
#include <SpheresEngine/EntityEngine/EntityEngine.h>
#include <SpheresEngine/Signals.h>

#include <gtest/gtest.h>

#include "TestEngines.h"

class TestEntity: public Entity {
public:
	slots::Slot<> m_slotSomeTest;
};

class TestAspect: public Aspect<TestEntity> {
public:
	virtual ~TestAspect() = default;

	void init(Engines &, TestEntity * sb) override {
		m_initCount++;
		sb->m_slotSomeTest.subscribe(
				[this] () {this->m_signalSomeTestCount++;});
	}

	size_t m_initCount = 0;
	size_t m_signalSomeTestCount = 0;
};

TEST(EntityEngineTest, entityWithAspect) {
	TestEntity et;
	TestEngines testEngines;
	auto engines = testEngines.getEngines();

	auto testAspect = std14::make_unique<TestAspect>();

	et.addAspect(engines, std::move(testAspect));

	ASSERT_EQ(
			static_cast<TestAspect*>(et.getAspectsBase()[0].get())->m_initCount,
			size_t(1));
	ASSERT_EQ(
			static_cast<TestAspect*>(et.getAspectsBase()[0].get())->m_signalSomeTestCount,
			size_t(0));

	// fire of signal
	et.m_slotSomeTest.signal();
	ASSERT_EQ(
			static_cast<TestAspect*>(et.getAspectsBase()[0].get())->m_signalSomeTestCount,
			size_t(1));

}
