#pragma once

#include <iostream>

#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/BulletCollision/Gimpact/btGImpactShape.h>
#include <bullet/BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h>
#include <glm/glm.hpp>

#include "PhysicsUtils.h"
#include "../Mesh.h"
#include "../Utils.h"

class PhysicsObject
{
public:
	//Constructors
	PhysicsObject();
	~PhysicsObject();

	//Update
	virtual void update();

	//Collision object
	btCollisionObject* getCollisionObject();
	btCollisionShape* getCollisionShape();

	//Mesh
	Mesh* getAttachedMesh();

	//Restitution
	void setRestitution(float restitution);
	float getRestitution();

	//Friction
	void setFriction(float friction);
	float getFriction();

	//Transformation methods
	glm::mat4 getTransformationMatrix();
	void updateTransformationMatrix();

	//Translation
	glm::vec3 getTranslation();
	void translateX(float amount);
    void translateY(float amount);
    void translateZ(float amount);
    void translate(glm::vec3 vector);
    void setTranslationX(float amount);
    void setTranslationY(float amount);
    void setTranslationZ(float amount);
    void setTranslation(glm::vec3 vector);

	//Scale
	glm::vec3 getScale();
	void scaleX(float amount);
    void scaleY(float amount);
    void scaleZ(float amount);
    void scale(float amount);
    void scale(glm::vec3 vector);
    void setScale(float amount);
    void setScale(glm::vec3 vector);

	//Rotation
    void yaw(float angle);
    void pitch(float angle);
    void roll(float angle);
    void rotate(glm::vec3 axis, float angle);
    void setRotation(glm::vec3 axis, float angle);

protected:
	Mesh* attachedMesh;
	btCollisionShape* collisionShape;
	btCollisionObject* collisionObject;
	glm::mat4 transformationMatrix;
};

