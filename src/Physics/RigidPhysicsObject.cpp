#include "RigidPhysicsObject.h"

RigidPhysicsObject::RigidPhysicsObject(BaseMesh* baseMesh) : PhysicsObject()
{
	this->attachedMesh = new Mesh(baseMesh);
}
RigidPhysicsObject::RigidPhysicsObject(PRIMITIVE_TYPE type, BaseMesh* baseMesh,
	float mass, float restitution, float friction) : PhysicsObject()
{
	this->attachedMesh = new Mesh(baseMesh);
	if(type == PRIMITIVE_BOX)
	{
		this->collisionShape = new btBoxShape(btVector3(1.0f,1.0f,1.0f));
	}
	else if(type == PRIMITIVE_SPHERE)
	{
		this->collisionShape = new btSphereShape(1.0f);
	}
	else if(type == PRIMITIVE_MESH)
	{
		std::cout << "Should not be creating a triangle mesh here!" << std::endl;
	}
	this->collisionShape->setUserPointer((void*)0);
	this->createRigidBody(mass,friction,restitution);
	//this->collisionObject->activate(true);
}
RigidPhysicsObject::~RigidPhysicsObject(){}

//RigidBody
void RigidPhysicsObject::createRigidBody(float mass, float friction, float restitution)
{
	bool isDynamic = (mass != 0.f);
	btVector3 localInertia(0,0,0);
	if (isDynamic)
		this->collisionShape->calculateLocalInertia(mass,localInertia);

	//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* myMotionState = new btDefaultMotionState(btTransform::getIdentity());
	btRigidBody::btRigidBodyConstructionInfo cInfo(mass,myMotionState,this->collisionShape,localInertia);
	this->collisionObject = new btRigidBody(cInfo);
	this->collisionObject->setContactProcessingThreshold(BT_LARGE_FLOAT);
	this->collisionObject->setFriction(friction);
	this->collisionObject->setRestitution(restitution);
	this->collisionObject->setCcdMotionThreshold(.1f);
	this->collisionObject->setCcdSweptSphereRadius(.1f);
}

//Update
void RigidPhysicsObject::update()
{
	PhysicsObject::update();
	//this->collisionObject->activate(true);
}