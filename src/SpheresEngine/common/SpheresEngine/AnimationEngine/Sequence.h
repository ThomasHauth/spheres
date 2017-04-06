#pragma once

#include <SpheresEngine/AnimationEngine/Sequence.h>
#include <boost/math/constants/constants.hpp>

#include <cmath>
#include <functional>
#include <vector>

/**
 * Defines a sequence lambda which is execute within a animation
 * sequence. First parameter is the current timestamp and second is the
 * total runtime of this sequence. A the beginning,
 * first par. / second par. = 0.0
 * and at the end of the sequence
 * first par. / second par. = 1.0
 */
typedef std::function<void(float, float)> SequenceLambda;

/**
 * Expresses one sequence of an animation with a fixed duration and a lambda
 * expression which can perform the animation work during each animation step
 * (like moving an entity).
 */
class Sequence {
public:
	/**
	 * Create a new sequence
	 *
	 * @param duration total duration of the sequence in seconds
	 * @param lmd Lambda function which executes the sequence's work
	 */
	Sequence(float duration, SequenceLambda lmd) :
			m_duration(duration), m_lmd(lmd) {

	}

	/**
	 * Execute the sequence lambda for this sequence
	 * @param sequenceTime is the
	 */
	void step(float sequenceTime) {
		m_lmd(sequenceTime, m_duration);
	}

	/**
	 * Get th total duration of this sequence
	 */
	float getDuration() const {
		return m_duration;
	}

private:
	float m_duration;
	SequenceLambda m_lmd;
};

namespace SequenceModifier {
// most trivial case of user-specific sequence, just linear
SequenceLambda buildLinear(SequenceLambda userLambda);
// tanh decay
SequenceLambda buildTanh(SequenceLambda userLambda);
}

typedef std::vector<Sequence> SequenceList;

