#pragma once

#include <glm/glm.hpp>
#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/BulletCollision/CollisionShapes/btConeShape.h>
#include <bullet/BulletCollision/CollisionShapes/btCapsuleShape.h>

#include <vector>

/*------------------------------------------
This class contains information about
different types of physics primitives,
including Spheres, Cubes, Meshes, and more
------------------------------------------*/

struct PrimitiveData
{
	PrimitiveData(){}
	virtual ~PrimitiveData(){}
	btCollisionShape* collisionShape;
};

//Box
struct PrimitiveDataBox : public PrimitiveData
{
	glm::vec3 dimensions;
	PrimitiveDataBox(glm::vec3 dimensions) : PrimitiveData() 
	{
		this->dimensions = dimensions;
		btVector3 dimensionsBullet = btVector3(dimensions.x,dimensions.y,dimensions.z);
		this->collisionShape = new btBoxShape(dimensionsBullet);
	}
};

//Sphere
struct PrimitiveDataSphere : public PrimitiveData
{
	float radius;
	PrimitiveDataSphere(float radius) : PrimitiveData() 
	{
		this->radius = radius;
		this->collisionShape = new btSphereShape(radius);
	}
};