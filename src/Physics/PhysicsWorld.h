#pragma once

#include <vector>
#include <iostream>

#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/BulletSoftBody/btSoftRigidDynamicsWorld.h>
#include <bullet/BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.h>
#include <bullet/BulletSoftBody/btSoftBodyHelpers.h>

#include "TriangleMeshPhysicsObject.h"
#include "PhysicsObject.h"
#include "RigidPhysicsObject.h"
#include "SoftPhysicsObject.h"
#include "../Utils.h"

class PhysicsWorld
{
public:
	PhysicsWorld();
	~PhysicsWorld();

	void addRigidObject(RigidPhysicsObject* object);
	void addSoftObject(SoftPhysicsObject* object);

	void update();
	
	btDefaultCollisionConfiguration* collisionConfiguration;
	btSoftRigidDynamicsWorld* world;
	btBroadphaseInterface*	broadphase;
	btCollisionDispatcher*	dispatcher;
	btConstraintSolver*	solver;
	
	std::vector<PhysicsObject*> objects;
	std::vector<SoftPhysicsObject*> softObjects;
	std::vector<RigidPhysicsObject*> rigidObjects;

private:
	void processCollisions();
	bool processCollision(TriangleMeshPhysicsObject* ob, btRigidBody* other, btManifoldPoint& pt, btVector3& pos, btVector3& normal);
	void refitMesh(TriangleMeshPhysicsObject* object);
	void dampenBounce(btRigidBody* ob, btManifoldPoint& pt, btVector3& normal);
	TriangleMeshPhysicsObject* convertRigidBodyToTriangleMeshPhysicsObject(btRigidBody* object);
};