#pragma once

#include <iostream>

#include <glm/glm.hpp>
#include <bullet/BulletSoftBody/btSoftBodyHelpers.h>

#include "PhysicsObject.h"

class SoftPhysicsObject : public PhysicsObject
{
public:

	//Constructors
	SoftPhysicsObject(BaseMesh* baseMesh, float mass, float restitution, float friction);
	~SoftPhysicsObject();
};

