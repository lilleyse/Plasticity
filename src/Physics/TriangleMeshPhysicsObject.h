#pragma once

#include <bullet/BulletCollision/Gimpact/btGImpactShape.h>
#include <bullet/BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h>

#include "RigidPhysicsObject.h"

struct PlasticityMaterial
{
	float threshold;
	float maxMagnitude;
	float breadth;
	float malleability;
	float falloff;
};

class TriangleMeshPhysicsObject : public RigidPhysicsObject
{
public:
	TriangleMeshPhysicsObject(BaseMesh* baseMesh, float mass, float restitution, float friction, bool dentable, PlasticityMaterial* material);
	~TriangleMeshPhysicsObject();

	PlasticityMaterial* material;
	bool dentable;
};