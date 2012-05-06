#include "TriangleMeshPhysicsObject.h"

TriangleMeshPhysicsObject::TriangleMeshPhysicsObject(BaseMesh* baseMesh, float mass, 
	float restitution, float friction, bool dentable, PlasticityMaterial* material) 
	: RigidPhysicsObject(baseMesh)
{
	this->material = material;
	this->dentable = dentable;

	int numTriangles = baseMesh->elementArray.size()/3;
	int vertexStride = sizeof(Vertex);
	int indexStride = 3*sizeof(int);
	int numVertices = baseMesh->numVertices;

	btTriangleIndexVertexArray* indexVertexArrays = new btTriangleIndexVertexArray(
		numTriangles,attachedMesh->getElements(),indexStride,numVertices,
		(float*)attachedMesh->getVertices(),vertexStride);

	if(mass == 0)
	{
		float dim = 100000;
		btVector3 aabbMin(-dim,-dim,-dim);
		btVector3 aabbMax(dim,dim,dim);
		btBvhTriangleMeshShape* trimesh = new btBvhTriangleMeshShape(indexVertexArrays,true,aabbMin,aabbMax);
		trimesh->setUserPointer(this);
		this->collisionShape = trimesh;
	}
	else
	{
		btGImpactMeshShape* trimesh = new btGImpactMeshShape(indexVertexArrays);
		trimesh->updateBound();
		trimesh->setUserPointer(this);
		this->collisionShape = trimesh;
	}
	this->createRigidBody(mass,friction,restitution);
	//this->collisionObject->activate(true);
}
TriangleMeshPhysicsObject::~TriangleMeshPhysicsObject(){}