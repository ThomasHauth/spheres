#include "Sequence.h"

// most trivial case of user-specific sequence, just linear
SequenceLambda SequenceModifier::buildLinear(SequenceLambda userLambda) {
	return [ userLambda ] ( float curTime, float duration) -> void
	{	return userLambda(curTime, duration);};
}

// tanh decay
SequenceLambda SequenceModifier::buildTanh(SequenceLambda userLambda) {
	return [ userLambda ] ( float curTime, float duration) -> void
	{
		const auto tanh_factor = std::tanh( (curTime / duration ) * boost::math::constants::pi<float>());
		return userLambda(curTime * tanh_factor, duration);};
}

