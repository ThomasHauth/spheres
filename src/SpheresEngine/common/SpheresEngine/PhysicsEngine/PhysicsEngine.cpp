#include <SpheresEngine/PhysicsEngine/PhysicsEngine.h>
#include <SpheresEngine/EntityEngine/CommonEntities/CollisionMixin.h>

void PhysicsEngine::addEntity(Entity * et) {
	// ensure this entity supports either physics or collisions
	CollisionMixin * cmixin = dynamic_cast<CollisionMixin*> (et);
	if (!cmixin) {
		logging::Fatal() << "Entity added to physics engine does not support collisions.";
	}
}
