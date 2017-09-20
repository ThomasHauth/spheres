#pragma once

#include <SpheresEngine/EntityEngine/Entity.h>

/**
 * Place holder for the physics engine which will be added later
 */
class PhysicsEngine {
public:
	/**
	 * not implemented atm.
	 */
	void step(float) {
	}

	/*
	 * Setup and include an entity for the physics simulation step
	 */
	void addEntity( Entity * et );
};
