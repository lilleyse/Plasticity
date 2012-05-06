#include "SoftPhysicsObject.h"

SoftPhysicsObject::SoftPhysicsObject(BaseMesh* baseMesh, int bendingConstraints, int iterations, float mass, float restitution, float friction)
{
	this->attachedMesh = new Mesh(baseMesh);
	int numTriangles = baseMesh->elementArray.size()/3;
	int numVertices = baseMesh->vertices.size();
	Vertex* vertexData = this->attachedMesh->getVertices();

	float *vertices = new float[numVertices*3];
	for(int i = 0; i < numVertices; i++)
	{
		Vertex* currentVertex = vertexData + i;
		vertices[i*3 + 0] = currentVertex->x;
		vertices[i*3 + 1] = currentVertex->y;
		vertices[i*3 + 2] = currentVertex->z;
	}

	btSoftBodyWorldInfo nullWorldInfo;
	this->collisionObject = btSoftBodyHelpers::CreateFromTriMesh(
		nullWorldInfo,vertices,this->attachedMesh->getElements(),numTriangles);
	btSoftBody* softBody = ((btSoftBody*)this->collisionObject);
	btSoftBody::Material* material = softBody->appendMaterial();
	softBody->generateBendingConstraints(bendingConstraints,material); //2 -- 10
	softBody->m_cfg.piterations = iterations; //2 -- 20
	softBody->m_cfg.kDF	= friction;
	softBody->m_cfg.collisions |= btSoftBody::fCollision::VF_SS;
	softBody->randomizeConstraints();
	softBody->setTotalMass(mass,true);

	//this->collisionObject->activate(true);
}
SoftPhysicsObject::~SoftPhysicsObject(){}

//Update
void SoftPhysicsObject::update()
{
	Vertex* vertexData = this->attachedMesh->getVertices();
	btSoftBody* softBody = ((btSoftBody*)this->collisionObject);
	for(int i = 0; i < softBody->m_nodes.size(); i++)
	{
		const btSoftBody::Node& node = softBody->m_nodes[i];
		Vertex* currentVertex = vertexData + i;
		currentVertex->x = node.m_x[0];
		currentVertex->y = node.m_x[1];
		currentVertex->z = node.m_x[2];
		currentVertex->nx = node.m_n[0];
		currentVertex->ny = node.m_n[1];
		currentVertex->nz = node.m_n[2];
	}
	this->attachedMesh->updateVertices();
}