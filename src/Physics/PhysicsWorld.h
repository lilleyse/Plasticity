#pragma once

#include <vector>

#include <bullet/btBulletDynamicsCommon.h>

#include "PhysicsObject.h"

class PhysicsWorld
{
public:
	PhysicsWorld();
	~PhysicsWorld();

	void addObject(PhysicsObject* object);
	void update();
	std::vector<PhysicsObject*>& getObjects();

private:
	btDefaultCollisionConfiguration* collisionConfiguration;
	btDynamicsWorld* dynamicsWorld;
	btBroadphaseInterface*	broadphase;
	btCollisionDispatcher*	dispatcher;
	btConstraintSolver*	solver;

	std::vector<PhysicsObject*> objects;
};