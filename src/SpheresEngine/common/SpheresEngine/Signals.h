#pragma once

#include <string>
#include <map>
#include <list>
#include <vector>
#include <memory>
#include <algorithm>
#include <functional>

#include <boost/noncopyable.hpp>
#include <SpheresEngine/Performance/SectionTimer.h>
#include <SpheresEngine/Timing.h>

// allows to optionally report the timing for each of the signal calls
//#define DESCENT_SIGNAL_PROFILE

namespace slots {

/**
 * List hold timings of signal calls
 */
typedef std::list<float> TimingQueue;

/**
 * Holds the slot name and the timing for the call
 */
typedef std::pair<std::string, float> TimingResult;

/**
 * List to hold multiple timing results
 */
typedef std::vector<TimingResult> TimingResultList;

/**
 * Non-templated base class so unsubscribing of multiple signals can be handled
 * in one place
 */
class SlotBase {
public:

	virtual ~SlotBase() {
	}

	/**
	 * Unique number by which a specific subscription can be identified
	 * (and later used to remove a specific subscription)
	 */
	typedef unsigned int subscribed_id;

	virtual void unsubscribe(subscribed_id id) = 0;

};

/**
 * Class to create a Slot where users can subscribe to
 * and get notified once the signal() method is called
 */
template<typename ... Args>
class Slot: public SlotBase {
public:

	/**
	 * type to store the lambda expression with a variable amount of
	 * parameters.
	 */
	typedef std::function<void(Args...)> func_type;

	/**
	 * Call this method with your custom lambda expression to subscribe
	 * to this signal slot.
	 * Name will not be stored, if the signal profiling is not enabled
	 */
	subscribed_id subscribe(func_type f, std::string name = "") {
		auto freeId = nextFreeId();
		m_functions.emplace_back(SlotFunction(freeId, name, f));

		// provide id to remove this subscribed
		return freeId;
	}

	void unsubscribe(subscribed_id id) override {
		m_functions.erase(
				std::remove_if(m_functions.begin(), m_functions.end(),
						[id]( SlotFunction & sf ) {return sf.getId() == id;}),
				m_functions.end());
	}

	/**
	 * Call this method to send a signal to all lambdas registered to this slot
	 */
	void signal(Args ... args) {
		for (auto const& f : m_functions) {
			//logging::Debug() << "signaling";
#ifdef DESCENT_SIGNAL_PROFILE
			Timing t;
#endif

			{
#ifdef DESCENT_SIGNAL_PROFILE
				SectionTimer tm(GlobalTimingRepo::Rep, "signal." + f.getName());
#endif
				f.getFunction()(args...);
			}

#ifdef DESCENT_SIGNAL_PROFILE

			addTiming(f.getName(), t.end());
#endif
		}
	}

private:

	/**
	 * Find the smallest free id
	 */
	subscribed_id nextFreeId() const {
		subscribed_id smallestFree = 0;

		bool didIncrease = true;

		// not the most optimal algorithm but will not fragment id space
		// and works on unsorted lists
		while (didIncrease) {

			didIncrease = false;
			for (auto const& f : m_functions) {
				if (f.getId() == smallestFree) {
					// check the next Id
					smallestFree++;
					didIncrease = true;
					// exit for loop, but still stay in the while loop
					break;
				}
			}
			// ok, all other entries checked and no coninue called ?
			// while loop will exit
		}

		return smallestFree;
	}

	/**
	 * Internal class to store tha name and the lambda expression of each
	 * registered subscriber
	 */
	class SlotFunction {
	public:
		/**
		 * Create a new slot function clsas using the name for profiling
		 * and the lambda expression
		 */
		SlotFunction(subscribed_id id, std::string const& name, func_type func) :
				m_id(id), m_function(func)
#ifdef DESCENT_SIGNAL_PROFILE
		, m_name(name)
#endif

		{

		}

		/**
		 * Return the stored lambda expression
		 */
		func_type getFunction() const {
			return m_function;
		}

		subscribed_id getId() const {
			return m_id;
		}

	private:

		/**
		 * Stored lambda expression
		 */
		func_type m_function;

		subscribed_id m_id;

#ifdef DESCENT_SIGNAL_PROFILE
	public:
		std::string m_name;
		std::string const& getName() const
		{
			return m_name;
		}
#endif

	};

	/**
	 * List of all functions stored as subscribers to this signal
	 */
	std::vector<SlotFunction> m_functions;

#ifdef DESCENT_SIGNAL_PROFILE

public:

	TimingResultList getTimingResultList() const {
		TimingResultList timingRes;

		timingRes.reserve(m_timings.size());

		for (auto tPairIt = m_timings.begin(); tPairIt != m_timings.end(); tPairIt++) {
			const float avg = average(tPairIt->second);
			timingRes.push_back(TimingResult(tPairIt->first, avg));
		}

		return timingRes;
	}

private:

	void addTiming(std::string name, float t) {
		TimingQueue & tq = m_timings[name];
		while (tq.size() > MaxTimingsCached) {
			tq.pop_front();
		}
		tq.push_back(t);
	}

	template<class TIterable>
	float average(TIterable const & theList) const {
		if (theList.size() == 0)
		return 0.0f;

		float sum = 0.0f;
		for (auto it = theList.begin(); it != theList.end(); it++) {
			sum += (*it);
		}
		return sum / theList.size();
	}

	std::map<std::string, TimingQueue> m_timings;
	constexpr static size_t MaxTimingsCached = 10;
#endif
};

}
