#pragma once

#include <bullet/btBulletDynamicsCommon.h>
#include <glm/glm.hpp>

#include "../Mesh.h"
#include "PhysicsUtils.h"

/*--------------------------------------
PhysicsObject has physics properties for
integration with Bullet.
--------------------------------------*/

enum PRIMITIVE_TYPE {
	PRIMITIVE_BOX,       //box
	PRIMITIVE_SPHERE,    //sphere
	PRIMITIVE_CYLINDER,  //cylinder
	PRIMITIVE_CONE,      //cone
	PRIMITIVE_CAPSULE,   //capsule
	PRIMITIVE_MESH,      //mesh
};

class PhysicsObject
{
public:

	//Constructors
	PhysicsObject(PRIMITIVE_TYPE type, float mass, float restitution, float friction);
	~PhysicsObject();

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

	//Scale
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

private:

	void createRigidBody(float mass, float friction, float restitution);

	Mesh* attachedMesh;
	btCollisionShape* collisionShape;
	btRigidBody* rigidBody;
	glm::mat4 transformationMatrix;
};

