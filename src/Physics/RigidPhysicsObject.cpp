#include "RigidPhysicsObject.h"

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
		int numTriangles = baseMesh->elementArray.size()/3;
		int vertexStride = sizeof(Vertex);
		int indexStride = 3*sizeof(int);
		int numVertices = baseMesh->vertices.size();

		btTriangleIndexVertexArray* indexVertexArrays = new btTriangleIndexVertexArray(
			numTriangles,
			attachedMesh->getElements(),
			indexStride,
			numVertices,
			(float*)attachedMesh->getVertices(),
			vertexStride);

		if(mass == 0)
		{
			float dim = 100;
			btVector3 aabbMin(-dim,-dim,-dim);
			btVector3 aabbMax(dim,dim,dim);
			btBvhTriangleMeshShape* trimesh = new btBvhTriangleMeshShape(indexVertexArrays,true,aabbMin,aabbMax);
			trimesh->setUserPointer(this->getAttachedMesh());
			this->collisionShape = trimesh;
		}
		else
		{
			btGImpactMeshShape * trimesh = new btGImpactMeshShape(indexVertexArrays);
			trimesh->updateBound();
			trimesh->setUserPointer(this->getAttachedMesh());
			this->collisionShape = trimesh;
		}
	}
	this->createRigidBody(mass,friction,restitution);
}
PhysicsObject::~PhysicsObject(){}

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
	this->rigidBody = new btRigidBody(cInfo);
	this->rigidBody->setContactProcessingThreshold(BT_LARGE_FLOAT);
	this->rigidBody->setFriction(friction);
	this->rigidBody->setRestitution(restitution);
	this->rigidBody->setCcdMotionThreshold(.1f);
	this->rigidBody->setCcdSweptSphereRadius(.1f);
}