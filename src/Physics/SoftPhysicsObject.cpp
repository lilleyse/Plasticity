#include "SoftPhysicsObject.h"

SoftPhysicsObject::SoftPhysicsObject(BaseMesh* baseMesh, float mass, float restitution, float friction)
{
	int numTriangles = baseMesh->elementArray.size()/3;
	int vertexStride = sizeof(Vertex);
	int indexStride = 3*sizeof(int);
	int numVertices = baseMesh->vertices.size();

	this->collisionObject = btSoftBodyHelpers::CreateFromTriMesh(pdemo->m_softBodyWorldInfo,gVerticesBunny,
		&gIndicesBunny[0][0],
		BUNNY_NUM_TRIANGLES);
	btSoftBody::Material*	pm=psb->appendMaterial();
	pm->m_kLST				=	0.5;
	pm->m_flags				-=	btSoftBody::fMaterial::DebugDraw;
	psb->generateBendingConstraints(2,pm);
	psb->m_cfg.piterations	=	2;
	psb->m_cfg.kDF			=	0.5;
	psb->randomizeConstraints();
	psb->scale(btVector3(6,6,6));
	psb->setTotalMass(100,true);	
	pdemo->getSoftDynamicsWorld()->addSoftBody(psb);
	pdemo->m_cutting=true;
}
SoftPhysicsObject::~SoftPhysicsObject()
{

}