#include "PhysicsObject.h"

PhysicsObject::PhysicsObject(){}
PhysicsObject::~PhysicsObject(){}

void PhysicsObject::initializeSphere(float radius, float mass, float restitution, float friction)
{
	this->primitiveData = new PrimitiveDataSphere(radius);
	this->createRigidBody(mass,friction,restitution);
}
void PhysicsObject::initializeBox(glm::vec3 dimensions, float mass, float restitution, float friction)
{
	this->primitiveData = new PrimitiveDataBox(dimensions);
	this->createRigidBody(mass,friction,restitution);
}
void PhysicsObject::initializeMesh(float mass, float restitution, float friction)
{
	//Do nothing yet...
}

//RigidBody
void PhysicsObject::createRigidBody(float mass, float friction, float restitution)
{
	bool isDynamic = (mass != 0.f);
	btCollisionShape* collisionShape = this->primitiveData->collisionShape;

	btVector3 localInertia(0,0,0);
	if (isDynamic)
		collisionShape->calculateLocalInertia(mass,localInertia);

	//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* myMotionState = new btDefaultMotionState(btTransform::getIdentity());
	btRigidBody::btRigidBodyConstructionInfo cInfo(mass,myMotionState,collisionShape,localInertia);
	this->rigidBody = new btRigidBody(cInfo);
	this->rigidBody->setContactProcessingThreshold(BT_LARGE_FLOAT);
	this->rigidBody->setFriction(friction);
	this->rigidBody->setRestitution(restitution);
	this->rigidBody->setCcdMotionThreshold(.1f);
	this->rigidBody->setCcdSweptSphereRadius(.1f);
}
btRigidBody* PhysicsObject::getRigidBody()
{
	return this->rigidBody;
}

//Mesh
void PhysicsObject::attachMesh(Mesh* attachedMesh)
{
	this->attachedMesh = attachedMesh;
}
Mesh* PhysicsObject::getAttachedMesh()
{
	return this->attachedMesh;
}

//Mass
void PhysicsObject::setMass(float mass)
{
	this->rigidBody->setMassProps(mass,btVector3(0,0,0));
}
float PhysicsObject::getMass()
{
	return this->rigidBody->getInvMass();
}

//Restitution
void PhysicsObject::setRestitution(float restitution)
{
	this->rigidBody->setRestitution(restitution);
}
float PhysicsObject::getRestitution()
{
	return this->rigidBody->getRestitution();
}

//Friction
void PhysicsObject::setFriction(float friction)
{
	this->rigidBody->setFriction(friction);
}	
float PhysicsObject::getFriction()
{
	return this->rigidBody->getFriction();
}

/*----------------------------------------
			Transformations
----------------------------------------*/
glm::mat4 PhysicsObject::getTransformationMatrix()
{
	return this->transformationMatrix;
}
void PhysicsObject::updateTransformationMatrix()
{
	btVector3 btScaleAmount = this->rigidBody->getCollisionShape()->getLocalScaling();
	glm::mat4 scaleMat = glm::mat4();
	scaleMat[0][0] = btScaleAmount.getX();
	scaleMat[1][1] = btScaleAmount.getY();
	scaleMat[2][2] = btScaleAmount.getZ();

	btTransform bulletTransform = this->rigidBody->getWorldTransform();
	this->transformationMatrix = Utils::convertBulletTransformToGLM(bulletTransform) * scaleMat;
}

//Translation
void PhysicsObject::translateX(float amount)
{
	btTransform bulletTransform = this->rigidBody->getWorldTransform();
	btVector3 position = bulletTransform.getOrigin();
	position += btVector3(amount,0,0);
	bulletTransform.setOrigin(position);
	this->rigidBody->setWorldTransform(bulletTransform);
	this->updateTransformationMatrix();
}
void PhysicsObject::translateY(float amount)
{
    btTransform bulletTransform = this->rigidBody->getWorldTransform();
	btVector3 position = bulletTransform.getOrigin();
	position += btVector3(0,amount,0);
	bulletTransform.setOrigin(position);
	this->rigidBody->setWorldTransform(bulletTransform);
	this->updateTransformationMatrix();
}
void PhysicsObject::translateZ(float amount)
{
    btTransform bulletTransform = this->rigidBody->getWorldTransform();
	btVector3 position = bulletTransform.getOrigin();
	position += btVector3(0,0,amount);
	bulletTransform.setOrigin(position);
	this->rigidBody->setWorldTransform(bulletTransform);
	this->updateTransformationMatrix();
}
void PhysicsObject::translate(glm::vec3 vector)
{
    btTransform bulletTransform = this->rigidBody->getWorldTransform();
	btVector3 position = bulletTransform.getOrigin();
	position += btVector3(vector.x,vector.y,vector.z);
	bulletTransform.setOrigin(position);
	this->rigidBody->setWorldTransform(bulletTransform);
	this->updateTransformationMatrix();
}
void PhysicsObject::setTranslationX(float amount)
{
    btTransform bulletTransform = this->rigidBody->getWorldTransform();
	btVector3 position = bulletTransform.getOrigin();
	position += btVector3(amount,0,0);
	bulletTransform.setOrigin(position);
	this->rigidBody->setWorldTransform(bulletTransform);
	this->updateTransformationMatrix();
}
void PhysicsObject::setTranslationY(float amount)
{
    btTransform bulletTransform = this->rigidBody->getWorldTransform();
	btVector3 position = bulletTransform.getOrigin();
	position += btVector3(0,amount,0);
	bulletTransform.setOrigin(position);
	this->rigidBody->setWorldTransform(bulletTransform);
	this->updateTransformationMatrix();
}
void PhysicsObject::setTranslationZ(float amount)
{
    btTransform bulletTransform = this->rigidBody->getWorldTransform();
	btVector3 position = bulletTransform.getOrigin();
	position += btVector3(0,0,amount);
	bulletTransform.setOrigin(position);
	this->rigidBody->setWorldTransform(bulletTransform);
	this->updateTransformationMatrix();
}
void PhysicsObject::setTranslation(glm::vec3 vector)
{
	btTransform bulletTransform = this->rigidBody->getWorldTransform();
	btVector3 position = btVector3(vector.x,vector.y,vector.z);
	bulletTransform.setOrigin(position);
	this->rigidBody->setWorldTransform(bulletTransform);
	this->updateTransformationMatrix();
}
glm::vec3 PhysicsObject::getTranslation()
{
    return glm::vec3(this->transformationMatrix[3]);
}

//Rotation
void PhysicsObject::yaw(float angle)
{
	btTransform bulletTransform = this->rigidBody->getWorldTransform();
	btQuaternion oldOrientation = bulletTransform.getRotation();
	
	btQuaternion newOrientation(btVector3(0,1,0),angle);
	newOrientation = oldOrientation + newOrientation;

	bulletTransform.setRotation(newOrientation);
	this->rigidBody->setWorldTransform(bulletTransform);
	this->transformationMatrix = Utils::convertBulletTransformToGLM(bulletTransform);
}
void PhysicsObject::pitch(float angle)
{
    btTransform bulletTransform = this->rigidBody->getWorldTransform();
	btQuaternion oldOrientation = bulletTransform.getRotation();
	
	btQuaternion newOrientation(btVector3(1,0,0),angle);
	newOrientation = oldOrientation + newOrientation;

	bulletTransform.setRotation(newOrientation);
	this->rigidBody->setWorldTransform(bulletTransform);
	this->transformationMatrix = Utils::convertBulletTransformToGLM(bulletTransform);
}
void PhysicsObject::roll(float angle)
{
    btTransform bulletTransform = this->rigidBody->getWorldTransform();
	btQuaternion oldOrientation = bulletTransform.getRotation();
	
	btQuaternion newOrientation(btVector3(0,0,1),angle);
	newOrientation = oldOrientation + newOrientation;

	bulletTransform.setRotation(newOrientation);
	this->rigidBody->setWorldTransform(bulletTransform);
	this->transformationMatrix = Utils::convertBulletTransformToGLM(bulletTransform);
}
void PhysicsObject::rotate(glm::vec3 axis, float angle)
{
    btTransform bulletTransform = this->rigidBody->getWorldTransform();
	btQuaternion oldOrientation = bulletTransform.getRotation();
	
	axis = glm::normalize(axis);
	btQuaternion newOrientation(btVector3(axis.x,axis.y,axis.z),angle);
	newOrientation = oldOrientation + newOrientation;

	bulletTransform.setRotation(newOrientation);
	this->rigidBody->setWorldTransform(bulletTransform);
	this->transformationMatrix = Utils::convertBulletTransformToGLM(bulletTransform);
}
void PhysicsObject::setRotation(glm::vec3 axis, float angle)
{
    btTransform bulletTransform = this->rigidBody->getWorldTransform();
	axis = glm::normalize(axis);
	btQuaternion newOrientation(btVector3(axis.x,axis.y,axis.z),angle);
	bulletTransform.setRotation(newOrientation);
	this->rigidBody->setWorldTransform(bulletTransform);
	this->transformationMatrix = Utils::convertBulletTransformToGLM(bulletTransform);
}