#pragma once

#include <bullet/btBulletDynamicsCommon.h>
#include <glm/glm.hpp>

#include "../Mesh.h"
#include "PhysicsPrimitives.h"
#include "PhysicsUtils.h"

/*--------------------------------------
PhysicsObject has physics properties for
integration with Bullet.
--------------------------------------*/

class PhysicsObject
{
public:

	//Constructors
	PhysicsObject();
	~PhysicsObject();

	//Create new PrimitiveData
	//Each calls createRigidBody
	void initializeSphere(float radius, float mass, float restitution, float friction);
	void initializeBox(glm::vec3 dimensions, float mass, float restitution, float friction);
	void initializeMesh(float mass, float restitution, float friction); //undefined

	//Rigidbody
	btRigidBody* getRigidBody();

	//Mesh
	void attachMesh(Mesh* attachedMesh);
	Mesh* getAttachedMesh();

	//Mass
	void setMass(float mass);
	float getMass();
	
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

	//Rotation
    void yaw(float angle);
    void pitch(float angle);
    void roll(float angle);
    void rotate(glm::vec3 axis, float angle);
    void setRotation(glm::vec3 axis, float angle);

private:

	void createRigidBody(float mass, float friction, float restitution);

	Mesh* attachedMesh;
	btRigidBody* rigidBody;
	PrimitiveData* primitiveData;
	glm::mat4 transformationMatrix;
};

