#pragma once

#include <boost/noncopyable.hpp>
#include <list>
#include <utility>

#include <SpheresEngine/AnimationEngine/Animation.h>

/**
 * This is the engine class which serves as entry point to the animation system
 *
 * animation system could also use a integer timing system to be faster and save memory...
 */
class AnimationEngine: boost::noncopyable {
public:

	/**
	 * add an animation and absolute start time
	 * @param animation An animation class which has already been filled
	 * with the sequences comprising the animation
	 * @param startTime the absolute start time of the animation in seconds
	 */
	void addAnimation(Animation animation, float startTime);

	/**
	 * Executes one timestep of the animation engine and all its animations
	 * todo: auto-clear animations which are done
	 *
	 * @param deltaT time difference in seconds since the last call to step()
	 */
	void step(const float deltaT);

private:

	/**
	 * Convenience typedef to store animations and their start times
	 */
	typedef std::pair<float, Animation> StartTimeAnimationPair;

	/**
	 * list of pairs of absolute animation start times (in seconds) and the
	 * animation object
	 */
	std::list<StartTimeAnimationPair> m_animations;

	/** local counter summing up the elapsed time to know when to start the
	 * animations
	 */
	float m_totalTime = 0.0f;
};
