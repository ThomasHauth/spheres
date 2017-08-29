#pragma once

#include <utility>
#include <cassert>
#include <cmath>
#include <map>
#include <memory>
#include <functional>

#include <boost/range/irange.hpp>
#include "VectorTypes.h"

/**
 * This using allows to use "own" in all places where a uniqe_ptr also
 * is owned by the code context holding the unique_ptr object
 */
template<class T>
using own = std::unique_ptr<T>;

namespace std14 {

/**
 * Shortcut to have c++14's make_unique also in c++11 compiles
 */
template<class T, class ... Args>
std::unique_ptr<T> make_unique(Args&&... args) {
	return std::unique_ptr < T > (new T(std::forward<Args>(args)...));
}
}

/**
 * Shortcut for using the unique_ptr class
 */
template<class T> using uniq = std::unique_ptr<T>;

namespace util {

/**
 * Class which holds a value and also stores if the value is valid, because
 * it has been set before
 */
template<class TVal>
class ValidValue {
public:

	/**
	 * Default constructor which mean the value will not be set
	 */
	ValidValue() = default;

	/**
	 * Create the class an set the value directly
	 */
	ValidValue(TVal const& v) :
			m_isValid(true), m_value(v) {
	}

	/**
	 * Simply returns an invalid instance of this class. This can be
	 * used to be more explicit than just using the default ctor
	 */
	static ValidValue<TVal> invalid() {
		return ValidValue<TVal>();
	}

	/**
	 * Set the value held in this class
	 */
	void setValue(TVal const& v) {
		m_value = v;
		m_isValid = true;
	}

	/**
	 * Return the value held by this class. Will produce
	 * a assert if the vaule has not been set before
	 */
	TVal const& get() const {
		// one should never ask for an invalid value
		assert(isValid());
		return m_value;
	}

	/**
	 * Return true if the held vaule is valid
	 */
	bool isValid() const {
		return m_isValid;
	}

	/**
	 * assignment operator to transfer validity correcty
	 */
	ValidValue<TVal> & operator=(TVal const& other) {
		setValue(other);
		// by convention, always return *this
		return *this;
	}

private:
	/**
	 * True if the m_value member has been set
	 */
	bool m_isValid = false;

	/**
	 * Vaule held by this class
	 */
	TVal m_value;
};

/** class to run a certain computation only once and always return the result
 *
 * 		auto lmbDirection = [&pent, &gs] () -> float {
 *		constexpr float maxDirection = 0.9f;
 *
 *		// compute player location
 *		float direction = pent->getPosition().x()
 *		gs.getEngines().renderEngine().getScreenTransform().screenSizeInTiles().x();
 *		// shift to -0.7 to 0.7
 *		direction = (direction * maxDirection * 2.0f) - maxDirection;
 *		return direction;
 *		};
 *
 *		util::RunOnce<decltype( lmbDirection), float> direction(lmbDirection);
 */
template<class TLambda, class TResult>
class RunOnce {
public:
	/**
	 * Crete an instance of the run once class and provide
	 * the lambda which should be executed
	 */
	RunOnce(TLambda const lmb) :
			m_lmb(lmb), m_res(), m_wasRun(false) {

	}

	/**
	 * Runs the lamba expression if not done yet and return
	 * the result.
	 */
	TResult result() {
		if (m_wasRun)
			return m_res;

		m_res = m_lmb();
		m_wasRun = true;

		return m_res;
	}

private:
	/**
	 * Lambda expression to run once
	 */
	TLambda m_lmb;

	/**
	 * Stored result as the lambda expression was run
	 */
	TResult m_res;

	/**
	 * Stores whether the lambda was run and a result is available
	 */
	bool m_wasRun;
};

/**
 * Returns true, if a numeric value is within a acceptable range
 * @param valMust the expected value
 * @param val the value to check
 * @param delta the absolute distance acceptable between valMust
 * and val
 */
template<class TFloatType>
inline bool withinDelta(TFloatType const& valMust, TFloatType const& val,
		TFloatType const& delta) {
	return (std::abs(valMust - val) < delta);
}

/**
 * Returns true if two vectors stay within a fixed distance to each other
 */
inline bool withinDelta(Vector2 const& valMust, Vector2 const& val,
		float const& deltaSquared) {
	const float dist = (valMust - val).magSquared();
	return dist < deltaSquared;
}

}

