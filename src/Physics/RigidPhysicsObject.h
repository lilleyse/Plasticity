#pragma once

#include <iostream>

#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/BulletCollision/Gimpact/btGImpactShape.h>
#include <bullet/BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h>
#include <glm/glm.hpp>

#include "PhysicsObject.h"
#include "PhysicsUtils.h"
#include "../Utils.h"

enum PRIMITIVE_TYPE {
	PRIMITIVE_BOX,       //box
	PRIMITIVE_SPHERE,    //sphere
	PRIMITIVE_CYLINDER,  //cylinder
	PRIMITIVE_CONE,      //cone
	PRIMITIVE_CAPSULE,   //capsule
	PRIMITIVE_MESH,      //mesh
};

class RigidPhysicsObject : public PhysicsObject
{
public:

	//Constructors
	RigidPhysicsObject(PRIMITIVE_TYPE type, BaseMesh* baseMesh, float mass, float restitution, float friction);
	~RigidPhysicsObject();

protected:

	void createRigidBody(float mass, float friction, float restitution);
};

