#include "PhysicsWorld.h"

PhysicsWorld::PhysicsWorld()
{
	//Set up bullet objects
	this->collisionConfiguration = new btDefaultCollisionConfiguration();
	this->dispatcher = new btCollisionDispatcher(this->collisionConfiguration);
	this->broadphase = new btDbvtBroadphase();
	this->solver = new btSequentialImpulseConstraintSolver();
	
	//Set up the world
	this->dynamicsWorld = new btDiscreteDynamicsWorld(this->dispatcher,this->broadphase,this->solver,this->collisionConfiguration);
	this->dynamicsWorld->getSolverInfo().m_splitImpulse = true;
	this->dynamicsWorld->getSolverInfo().m_numIterations = 20;
	this->dynamicsWorld->getDispatchInfo().m_useContinuous = true;
	this->dynamicsWorld->setGravity(btVector3(0,-10,0));
}
PhysicsWorld::~PhysicsWorld(){}

void PhysicsWorld::update()
{
	this->dynamicsWorld->stepSimulation(1.0f/60.0f);

	int numManifolds = this->dynamicsWorld->getDispatcher()->getNumManifolds();
	for (int i=0;i<numManifolds;i++)
	{
		btPersistentManifold* contactManifold =  this->dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
		btCollisionObject* obA = static_cast<btCollisionObject*>(contactManifold->getBody0());
		btCollisionObject* obB = static_cast<btCollisionObject*>(contactManifold->getBody1());
	
		int numContacts = contactManifold->getNumContacts();
		for (int j=0;j<numContacts;j++)
		{
			btManifoldPoint& pt = contactManifold->getContactPoint(j);
			if (pt.getDistance()<0.f)
			{
				const btVector3& ptA = pt.getPositionWorldOnA();
				const btVector3& ptB = pt.getPositionWorldOnB();
				const btVector3& normalOnB = pt.m_normalWorldOnB;
				std::cout << pt.getAppliedImpulse() << std::endl;
			}
		}
	}

}
void PhysicsWorld::addObject(PhysicsObject* object)
{
	this->dynamicsWorld->addRigidBody(object->getRigidBody());
	this->objects.push_back(object);
}
std::vector<PhysicsObject*>& PhysicsWorld::getObjects()
{
	return this->objects;
}

