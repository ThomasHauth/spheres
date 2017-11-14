#include <SpheresEngine/PhysicsEngine/PhysicsEngine.h>
#include <SpheresEngine/EntityEngine/CommonEntities/CollisionMixin.h>

PhysicsEngine::PhysicsEngine() {
	m_collisionConfig = std14::make_unique<btDefaultCollisionConfiguration>();
	m_dispatcher = std14::make_unique<btCollisionDispatcher>(
			m_collisionConfig.get());
	m_overlapPair = std14::make_unique<btDbvtBroadphase>();
	m_solver = std14::make_unique<btSequentialImpulseConstraintSolver>();
	m_world = std14::make_unique<btDiscreteDynamicsWorld>(m_dispatcher.get(),
			m_overlapPair.get(), m_solver.get(), m_collisionConfig.get());
}

void PhysicsEngine::addEntity(Entity * et) {
	// ensure this entity supports either physics or collisions
	CollisionMixin * cmixin = dynamic_cast<CollisionMixin*>(et);
	if (!cmixin) {
		logging::Fatal()
				<< "Entity added to physics engine does not support collisions.";
	}

	if (cmixin->getShapeSource() == CollisionMixin::ShapeSource::Box) {
		btCollisionShape* shape = new btBoxShape(
				BulletUtil::toBullet(cmixin->getBoxSize()));
		// todo: make sure the collision shape is stored and cleaned up later
		//collisionShapes.push_back(groundShape);

		btTransform groundTransform;
		groundTransform.setIdentity();
		groundTransform.setOrigin(btVector3(0, 0, 0));

		// not dynamic with mass 0
		btScalar mass(0.);
		btVector3 localInertia(0, 0, 0);

		//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* myMotionState = new btDefaultMotionState(
				groundTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState,
				shape, localInertia);
		btRigidBody* body = new btRigidBody(rbInfo);

		//add the body to the dynamics world
		m_world->addRigidBody(body);

		logging::Info() << "PhysicsEntity of shape box registered";
	} else {
		logging::Fatal() << "Unsupported collision shape";
	}

}
