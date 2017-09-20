#pragma once

//#include <SpheresEngine/EntityEngine/EntityEngine.h>
//#include <SpheresEngine/InputEngine/InputEngine.h>

class EntityEngine;
class InputEngine;
class RenderEngine;
class AnimationEngine;
class PhysicsEngine;

class Engines {
public:
	explicit Engines(EntityEngine & ee, InputEngine & ie, RenderEngine & re,
			AnimationEngine & ae, PhysicsEngine & pe) :
			entity(ee), input(ie), render(re), animation(ae), physics(pe) {

	}

	EntityEngine & entity;
	InputEngine & input;
	RenderEngine & render;
	AnimationEngine & animation;
	PhysicsEngine & physics;
};
