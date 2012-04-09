#pragma once

#include <glm/glm.hpp>
#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/BulletCollision/CollisionShapes/btConeShape.h>
#include <bullet/BulletCollision/CollisionShapes/btCapsuleShape.h>

#include "PhysicsUtils.h"

/*------------------------------------------
This class contains information about
different types of physics primitives,
including Spheres, Cubes, Meshes, and more
------------------------------------------*/

enum PRIMITIVE_TYPE {
	PRIMITIVE_BOX,       //box
	PRIMITIVE_SPHERE,    //sphere
	PRIMITIVE_CYLINDER,  //cylinder
	PRIMITIVE_CONE,      //cone
	PRIMITIVE_CAPSULE,   //capsule
	PRIMITIVE_MESH,      //mesh
};

struct PrimitiveData
{
	PrimitiveData(){}
	virtual ~PrimitiveData(){}
	btCollisionShape* collisionShape;
};

//Box
struct PrimitiveDataBox : public PrimitiveData
{
	PrimitiveDataBox(glm::vec3 dimensions) : PrimitiveData() 
	{
		btVector3 dimensionsBullet = Utils::convertGLMVectorToBullet(dimensions);
		this->collisionShape = new btBoxShape(dimensionsBullet);
	}
};

//Sphere
struct PrimitiveDataSphere : public PrimitiveData
{
	PrimitiveDataSphere(float radius) : PrimitiveData() 
	{
		this->collisionShape = new btSphereShape(radius);
	}
};