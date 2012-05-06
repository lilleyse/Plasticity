#include "PhysicsObject.h"

PhysicsObject::PhysicsObject(){}
PhysicsObject::~PhysicsObject(){}

//Update
void PhysicsObject::update(){}

//Collision Object
btCollisionObject* PhysicsObject::getCollisionObject()
{
	return this->collisionObject;
}

btCollisionShape* PhysicsObject::getCollisionShape()
{
	return this->collisionShape;
}

//Mesh
Mesh* PhysicsObject::getAttachedMesh()
{
	return this->attachedMesh;
}

//Restitution
void PhysicsObject::setRestitution(float restitution)
{
	this->collisionObject->setRestitution(restitution);
}
float PhysicsObject::getRestitution()
{
	return this->collisionObject->getRestitution();
}

//Friction
void PhysicsObject::setFriction(float friction)
{
	this->collisionObject->setFriction(friction);
}	
float PhysicsObject::getFriction()
{
	return this->collisionObject->getFriction();
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
	//Scaling is not part of the world transform, so we need to add it manually
	btVector3 btScaleAmount = this->collisionObject->getCollisionShape()->getLocalScaling();
	glm::mat4 scaleMat = glm::mat4();
	scaleMat[0][0] = btScaleAmount.getX();
	scaleMat[1][1] = btScaleAmount.getY();
	scaleMat[2][2] = btScaleAmount.getZ();

	btTransform bulletTransform = this->collisionObject->getWorldTransform();
	this->transformationMatrix = Utils::convertBulletTransformToGLM(bulletTransform) * scaleMat;
}

//Translation
void PhysicsObject::translateX(float amount)
{
	btTransform bulletTransform = this->collisionObject->getWorldTransform();
	btVector3 position = bulletTransform.getOrigin();
	position += btVector3(amount,0,0);
	bulletTransform.setOrigin(position);
	this->collisionObject->setWorldTransform(bulletTransform);
	this->updateTransformationMatrix();
}
void PhysicsObject::translateY(float amount)
{
    btTransform bulletTransform = this->collisionObject->getWorldTransform();
	btVector3 position = bulletTransform.getOrigin();
	position += btVector3(0,amount,0);
	bulletTransform.setOrigin(position);
	this->collisionObject->setWorldTransform(bulletTransform);
	this->updateTransformationMatrix();
}
void PhysicsObject::translateZ(float amount)
{
    btTransform bulletTransform = this->collisionObject->getWorldTransform();
	btVector3 position = bulletTransform.getOrigin();
	position += btVector3(0,0,amount);
	bulletTransform.setOrigin(position);
	this->collisionObject->setWorldTransform(bulletTransform);
	this->updateTransformationMatrix();
}
void PhysicsObject::translate(glm::vec3 vector)
{
    btTransform bulletTransform = this->collisionObject->getWorldTransform();
	btVector3 position = bulletTransform.getOrigin();
	position += btVector3(vector.x,vector.y,vector.z);
	bulletTransform.setOrigin(position);
	this->collisionObject->setWorldTransform(bulletTransform);
	this->updateTransformationMatrix();
}
void PhysicsObject::setTranslationX(float amount)
{
    btTransform bulletTransform = this->collisionObject->getWorldTransform();
	btVector3 position = bulletTransform.getOrigin();
	position += btVector3(amount,0,0);
	bulletTransform.setOrigin(position);
	this->collisionObject->setWorldTransform(bulletTransform);
	this->updateTransformationMatrix();
}
void PhysicsObject::setTranslationY(float amount)
{
    btTransform bulletTransform = this->collisionObject->getWorldTransform();
	btVector3 position = bulletTransform.getOrigin();
	position += btVector3(0,amount,0);
	bulletTransform.setOrigin(position);
	this->collisionObject->setWorldTransform(bulletTransform);
	this->updateTransformationMatrix();
}
void PhysicsObject::setTranslationZ(float amount)
{
    btTransform bulletTransform = this->collisionObject->getWorldTransform();
	btVector3 position = bulletTransform.getOrigin();
	position += btVector3(0,0,amount);
	bulletTransform.setOrigin(position);
	this->collisionObject->setWorldTransform(bulletTransform);
	this->updateTransformationMatrix();
}
void PhysicsObject::setTranslation(glm::vec3 vector)
{
	btTransform bulletTransform = this->collisionObject->getWorldTransform();
	btVector3 position = btVector3(vector.x,vector.y,vector.z);
	bulletTransform.setOrigin(position);
	this->collisionObject->setWorldTransform(bulletTransform);
	this->updateTransformationMatrix();
}
glm::vec3 PhysicsObject::getTranslation()
{
    return glm::vec3(this->transformationMatrix[3]);
}

//Scale
glm::vec3 PhysicsObject::getScale()
{
	btVector3 btScaleAmount = this->collisionObject->getCollisionShape()->getLocalScaling();
	return Utils::convertBulletVectorToGLM(btScaleAmount);
}
void PhysicsObject::scaleX(float amount)
{
    btVector3 scaleAmount = this->collisionObject->getCollisionShape()->getLocalScaling();
	scaleAmount += btVector3(amount,0,0);
	this->collisionObject->getCollisionShape()->setLocalScaling(scaleAmount);
	this->updateTransformationMatrix();
}
void PhysicsObject::scaleY(float amount)
{
    btVector3 scaleAmount = this->collisionObject->getCollisionShape()->getLocalScaling();
	scaleAmount += btVector3(0,amount,0);
	this->collisionObject->getCollisionShape()->setLocalScaling(scaleAmount);
	this->updateTransformationMatrix();
}
void PhysicsObject::scaleZ(float amount)
{
    btVector3 scaleAmount = this->collisionObject->getCollisionShape()->getLocalScaling();
	scaleAmount += btVector3(0,0,amount);
	this->collisionObject->getCollisionShape()->setLocalScaling(scaleAmount);
	this->updateTransformationMatrix();
}
void PhysicsObject::scale(float amount)
{
    btVector3 scaleAmount = this->collisionObject->getCollisionShape()->getLocalScaling();
	scaleAmount += btVector3(amount,amount,amount);
	this->collisionObject->getCollisionShape()->setLocalScaling(scaleAmount);
	this->updateTransformationMatrix();
}
void PhysicsObject::scale(glm::vec3 vector)
{
	btVector3 scaleAmount = this->collisionObject->getCollisionShape()->getLocalScaling();
	scaleAmount += btVector3(vector.x,vector.y,vector.z);
	this->collisionObject->getCollisionShape()->setLocalScaling(scaleAmount);
	this->updateTransformationMatrix();
}
void PhysicsObject::setScale(float amount)
{
	btVector3 scaleAmount = btVector3(amount,amount,amount);
    this->collisionObject->getCollisionShape()->setLocalScaling(scaleAmount);
	this->updateTransformationMatrix();
}
void PhysicsObject::setScale(glm::vec3 vector)
{
	btVector3 scaleAmount = btVector3(vector.x,vector.y,vector.z);
    this->collisionObject->getCollisionShape()->setLocalScaling(scaleAmount);
	this->updateTransformationMatrix();
}

//Rotation
void PhysicsObject::yaw(float angle)
{
	btTransform bulletTransform = this->collisionObject->getWorldTransform();
	btQuaternion oldOrientation = bulletTransform.getRotation();
	
	btQuaternion newOrientation(btVector3(0,1,0),angle);
	newOrientation = oldOrientation + newOrientation;

	bulletTransform.setRotation(newOrientation);
	this->collisionObject->setWorldTransform(bulletTransform);
	this->transformationMatrix = Utils::convertBulletTransformToGLM(bulletTransform);
}
void PhysicsObject::pitch(float angle)
{
    btTransform bulletTransform = this->collisionObject->getWorldTransform();
	btQuaternion oldOrientation = bulletTransform.getRotation();
	
	btQuaternion newOrientation(btVector3(1,0,0),angle);
	newOrientation = oldOrientation + newOrientation;

	bulletTransform.setRotation(newOrientation);
	this->collisionObject->setWorldTransform(bulletTransform);
	this->transformationMatrix = Utils::convertBulletTransformToGLM(bulletTransform);
}
void PhysicsObject::roll(float angle)
{
    btTransform bulletTransform = this->collisionObject->getWorldTransform();
	btQuaternion oldOrientation = bulletTransform.getRotation();
	
	btQuaternion newOrientation(btVector3(0,0,1),angle);
	newOrientation = oldOrientation + newOrientation;

	bulletTransform.setRotation(newOrientation);
	this->collisionObject->setWorldTransform(bulletTransform);
	this->transformationMatrix = Utils::convertBulletTransformToGLM(bulletTransform);
}
void PhysicsObject::rotate(glm::vec3 axis, float angle)
{
    btTransform bulletTransform = this->collisionObject->getWorldTransform();
	btQuaternion oldOrientation = bulletTransform.getRotation();
	
	axis = glm::normalize(axis);
	btQuaternion newOrientation(btVector3(axis.x,axis.y,axis.z),angle);
	newOrientation = oldOrientation + newOrientation;

	bulletTransform.setRotation(newOrientation);
	this->collisionObject->setWorldTransform(bulletTransform);
	this->transformationMatrix = Utils::convertBulletTransformToGLM(bulletTransform);
}
void PhysicsObject::setRotation(glm::vec3 axis, float angle)
{
    btTransform bulletTransform = this->collisionObject->getWorldTransform();
	axis = glm::normalize(axis);
	btQuaternion newOrientation(btVector3(axis.x,axis.y,axis.z),angle);
	bulletTransform.setRotation(newOrientation);
	this->collisionObject->setWorldTransform(bulletTransform);
	this->transformationMatrix = Utils::convertBulletTransformToGLM(bulletTransform);
}