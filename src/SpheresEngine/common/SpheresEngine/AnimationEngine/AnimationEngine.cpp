#include <SpheresEngine/AnimationEngine/AnimationEngine.h>

void AnimationEngine::addAnimation(Animation animation, float startTime) {

	m_animations.push_back(std::make_pair(startTime, animation));

}

void AnimationEngine::step(const float deltaT) {
	m_totalTime += deltaT;
	for (auto & apair : m_animations) {
		if (apair.first < m_totalTime) {
			// subtract the start time of the whole animation
			// to allow for easier coordination within the animation
			apair.second.step(m_totalTime - apair.first);

			if (apair.second.isRetired() && apair.second.isAutoRepeat()) {
				// restart animation an new starting time
				apair.first = m_totalTime;
				apair.second.repeat();
			}
		}
	}
}
