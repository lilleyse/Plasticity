#pragma once

#include <vector>
#include <iostream>

#include <bullet/btBulletDynamicsCommon.h>

#include "PhysicsObject.h"
#include "../Utils.h"

class PhysicsWorld
{
public:
	PhysicsWorld();
	~PhysicsWorld();

	void addObject(PhysicsObject* object);
	void update();
	void processCollisions();
	std::vector<PhysicsObject*>& getObjects();

	btDefaultCollisionConfiguration* collisionConfiguration;
	btDynamicsWorld* dynamicsWorld;
	btBroadphaseInterface*	broadphase;
	btCollisionDispatcher*	dispatcher;
	btConstraintSolver*	solver;

private:
	std::vector<PhysicsObject*> objects;
};