#pragma once

#include <SpheresEngine/EntityEngine/Entity.h>

#include <btBulletDynamicsCommon.h>

#include <memory>

class BulletUtil {
public:

	static btVector3 toBullet(Vector3 v) {
		return btVector3(v.x(), v.y(), v.z());
	}

	static Vector3 toSpheres(btVector3 v) {
		return Vector3(v.x(), v.y(), v.z());
	}
};
/*
struct {
	Entity * Ent;

};
*/
/**
 * Place holder for the physics engine which will be added later
 */
class PhysicsEngine {
public:

	explicit PhysicsEngine();

	/**
	 * not implemented atm.
	 */
	void step(float) {
	}

	/*
	 * Setup and include an entity for the physics simulation step
	 */
	void addEntity(Entity * et);

private:

	// the order of the following members is important so the bullet objects
	// are destroyed reverse order of dependency
	std::unique_ptr<btDiscreteDynamicsWorld> m_world;
	std::unique_ptr<btSequentialImpulseConstraintSolver> m_solver;
	std::unique_ptr<btDbvtBroadphase> m_overlapPair;
	std::unique_ptr<btCollisionDispatcher> m_dispatcher;
	std::unique_ptr<btDefaultCollisionConfiguration> m_collisionConfig;
};
