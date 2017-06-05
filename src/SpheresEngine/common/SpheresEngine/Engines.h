#pragma once

//#include <SpheresEngine/EntityEngine/EntityEngine.h>
//#include <SpheresEngine/InputEngine/InputEngine.h>

class EntityEngine;
class InputEngine;
class RenderEngine;
class AnimationEngine;

class Engines {
public:
	explicit Engines(EntityEngine & ee, InputEngine & ie, RenderEngine & re, AnimationEngine & ae) :
			entity(ee), input(ie), render(re), animation(ae) {

	}

	EntityEngine & entity;
	InputEngine & input;
	RenderEngine & render;
	AnimationEngine & animation;
};
