#pragma once

#include "Sequence.h"

#include <list>
#include <iostream>

/**
 * This pair contains the timecode and the sequence which
 * is started at this timecode.
 */
typedef std::pair<float, Sequence> StartTimeSequencePair;

/**
 * Convenience type-def for StartTimeSequencePair users
 */
typedef std::list<StartTimeSequencePair> AnimationList;

/**
 * This class formulates an animation, which is comprised of the sequences and
 * the timescodes when they should be started. This class is also responsible for
 * stepping through the animation and keeping track of running animations
 * and retirering complete animations.
 */
class Animation {
public:
	/**
	 * Create an animation and fill the list of sequences and time code
	 * @param initialSequence list of timecode and sequence pairs
	 * @param autoRepeat automatically restart the animation once it
	 * has been completed
	 */
	Animation(AnimationList initialSequence, bool autoRepeat = false) :
			m_sequences(initialSequence), m_autoRepeat(autoRepeat) {
	}

	/** Support default ctor to allow the user to later fill the sequence list
	 * later
	 */
	Animation() = default;

	/**
	 * add a sequence and the timecode when to start the sequence
	 * @param startTime time in seconds when to start the animation
	 * @param sq The sequence to add
	 */
	void addSequence(float startTime, Sequence sq) {
		m_sequences.push_back(std::make_pair(startTime, sq));
	}

	/**
	 * Step trough the sequences of this animation
	 * todo: now the animation will start right away
	 * in the future, we may want to start delayed
	 *
	 * @param animationTime the relative time (to animation start) of the current
	 * step in seconds
	 */
	void step(float animationTime) {

		// the default is to retire the animation, if one sequence exists,
		// which is still running, the retired flag will be set to false
		m_retired = true;
		for (auto & s : m_sequences) {

			// can this sequence be retired ?
			const bool seqRetired = animationTime
					> (s.first + s.second.getDuration());
			m_retired &= seqRetired;

			if (!seqRetired && (animationTime > s.first)) {
				// pass only the sequence time, aka the relative time
				// wrt to the sequence absolute start value
				s.second.step(animationTime - s.first);
			}
		}
	}

	/**
	 * Return true if this whole animation has been retired
	 */
	bool isRetired() const {
		return m_retired;
	}

	/**
	 * Return true if this animation is set to auto-repeat
	 */
	bool isAutoRepeat() const {
		return m_autoRepeat;
	}

	/**
	 * Un-retire this animation so it can be executed again
	 */
	void repeat() {
		m_retired = false;
	}

private:
	/**
	 * List of sequences and their start time
	 */
	AnimationList m_sequences;

	/**
	 * Stores whether all sequences within this animation have been run
	 * and the whole animation is retired
	 */
	bool m_retired = false;

	/**
	 * Stores if the animation willl auto-magically restarted
	 */
	const bool m_autoRepeat = false;
};

