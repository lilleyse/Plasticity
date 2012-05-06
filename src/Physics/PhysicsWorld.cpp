#include "PhysicsWorld.h"

PhysicsWorld::PhysicsWorld()
{
	this->collisionConfiguration = new btSoftBodyRigidBodyCollisionConfiguration();
	this->dispatcher = new btCollisionDispatcher(this->collisionConfiguration);
	this->broadphase = new btDbvtBroadphase();
	this->solver = new btSequentialImpulseConstraintSolver();
	this->world = new btSoftRigidDynamicsWorld(this->dispatcher,this->broadphase,this->solver,this->collisionConfiguration);
	this->world->setGravity(btVector3(0,-10,0));
	btGImpactCollisionAlgorithm::registerAlgorithm(this->dispatcher);
}
PhysicsWorld::~PhysicsWorld(){}

void PhysicsWorld::update()
{
	this->world->stepSimulation(1.0f/60.0f);
	this->processCollisions();
}
void PhysicsWorld::processCollisions()
{
	int numManifolds = this->world->getDispatcher()->getNumManifolds();
	for (int i=0;i<numManifolds;i++)
	{
		btPersistentManifold* contactManifold =  this->world->getDispatcher()->getManifoldByIndexInternal(i);
		btRigidBody* obA = static_cast<btRigidBody*>(contactManifold->getBody0());
		btRigidBody* obB = static_cast<btRigidBody*>(contactManifold->getBody1());

		//check if obA and obB are dentable
		bool obADentable = false;
		TriangleMeshPhysicsObject* obATriMesh = this->convertRigidBodyToTriangleMeshPhysicsObject(obA);
		if(obATriMesh != 0 && obATriMesh->dentable)
			obADentable = true;

		bool obBDentable = false;
		TriangleMeshPhysicsObject* obBTriMesh = this->convertRigidBodyToTriangleMeshPhysicsObject(obB);
		if(obBTriMesh != 0 && obBTriMesh->dentable)
			obBDentable = true;



		int numContacts = contactManifold->getNumContacts();
		for (int j=0;j<numContacts;j++)
		{
			btManifoldPoint& pt = contactManifold->getContactPoint(j);
			int indexA = pt.m_index0;
			int indexB = pt.m_index1;
			btVector3 ptA = pt.m_localPointA;//pt.getPositionWorldOnA();
			btVector3 ptB = pt.m_localPointB;//pt.getPositionWorldOnB();
			btVector3 normalOnB = pt.m_normalWorldOnB;
			btVector3 normalOnA = -pt.m_normalWorldOnB;
			bool obADeformed = false;
			bool obBDeformed = false;
			if(obADentable) obADeformed = this->processCollision(obATriMesh,obB,pt,ptA,normalOnA);
			if(obBDentable) obBDeformed = this->processCollision(obBTriMesh,obA,pt,ptB,normalOnB);
			
			if(obADeformed || obBDeformed)
			{
				this->dampenBounce(obA,pt,normalOnA);
				this->dampenBounce(obB,pt,normalOnB);
			}
		}

		if(numContacts > 0)
		{
			if(obADentable) this->refitMesh(obATriMesh);
			if(obBDentable) this->refitMesh(obBTriMesh);
		}
	}
}

//Fixes the mesh shape if it is dentable
void PhysicsWorld::refitMesh(TriangleMeshPhysicsObject* object)
{
	//object->setActivationState(DISABLE_DEACTIVATION);

	btBvhTriangleMeshShape* bvhTrimesh = dynamic_cast<btBvhTriangleMeshShape*>(object->getCollisionShape());
	btGImpactMeshShape* gimpactTrimesh = dynamic_cast<btGImpactMeshShape*>(object->getCollisionShape());
	if(bvhTrimesh != 0)
	{
		float dim = 100000;
		btVector3 aabbMin(-dim,-dim,-dim);
		btVector3 aabbMax(dim,dim,dim);
		bvhTrimesh->refitTree(aabbMin,aabbMax);
	}
	else if(gimpactTrimesh != 0)
	{
		//gimpactTrimesh->postUpdate();
		//gimpactTrimesh->updateBound();

		//float dim = 100000;
		//btVector3 aabbMin(-dim,-dim,-dim);
		//btVector3 aabbMax(dim,dim,dim);
		//btTransform identity;
		//identity.setIdentity();
		//gimpactTrimesh->getAabb(identity,aabbMin,aabbMax);

		gimpactTrimesh->postUpdate();
		gimpactTrimesh->updateBound();
		//Might not work
			
			
	}
	//
	//this->world->getBroadphase()->getOverlappingPairCache()->cleanProxyFromPairs(obB->getBroadphaseHandle(),this->world->getDispatcher());
}
bool PhysicsWorld::processCollision(TriangleMeshPhysicsObject* ob, btRigidBody* other, btManifoldPoint& pt, btVector3& pos, btVector3& normal)
{
	PlasticityMaterial* material = ob->material;
	btVector3 projectileVelocity = other->getLinearVelocity();
	btVector3 projectileVelocityNorm = projectileVelocity;
	projectileVelocityNorm.normalize();
	btScalar projectileVelocityLength = projectileVelocity.length();
	float impulse = normal.dot(projectileVelocityNorm)*projectileVelocityLength;//pt.m_appliedImpulse;
	float distance = pt.getDistance();
	if (distance < 0.f && impulse > material->threshold)
	{
		//Update the positions for all neighbors around the intersection point
		glm::vec3 intersectionPos = Utils::convertBulletVectorToGLM(pos);
		float magnitude = std::min(impulse*material->malleability, material->maxMagnitude);
			
		Mesh* mesh = ob->getAttachedMesh();
		Vertex* vertices = mesh->getVertices();
		int numVertices = mesh->numVertices;
		for(int i = 0; i < numVertices; i++)
		{
			glm::vec3 vertex = glm::vec3(vertices[i].x, vertices[i].y, vertices[i].z);
			float distanceFromIntersection = glm::distance(intersectionPos,vertex);
			float displacement = (material->breadth - distanceFromIntersection)/material->breadth;
			displacement = glm::clamp(displacement, 0.0f, 1.0f);
			displacement = std::pow(displacement, material->falloff);
			vertices[i].x -= normal.getX()*magnitude*displacement;
			vertices[i].y -= normal.getY()*magnitude*displacement;
			vertices[i].z -= normal.getZ()*magnitude*displacement;
		}
		mesh->updateNormals();
		mesh->updateVertices();
		return true;
	}
	return false;
}

void PhysicsWorld::dampenBounce(btRigidBody* ob, btManifoldPoint& pt, btVector3& normal)
{
	//ob->setLinearVelocity(-ob->getLinearVelocity()/2);
	ob->applyCentralForce(normal*5*pt.m_appliedImpulse);
}

TriangleMeshPhysicsObject* PhysicsWorld::convertRigidBodyToTriangleMeshPhysicsObject(btRigidBody* object)
{
	btCollisionShape* trimesh = object->getCollisionShape();
	void* userData = trimesh->getUserPointer();
	if(userData != 0)
	{
		TriangleMeshPhysicsObject* physicsObject = (TriangleMeshPhysicsObject*)(userData);
		return physicsObject;
	}
	return 0;
}
void PhysicsWorld::addRigidObject(RigidPhysicsObject* object)
{
	this->world->addRigidBody((btRigidBody*)object->getCollisionObject());
	this->objects.push_back(object);
	this->rigidObjects.push_back(object);
}
void PhysicsWorld::addSoftObject(SoftPhysicsObject* object)
{
	btSoftBody* softBody = (btSoftBody*)object->getCollisionObject();
	softBody->m_worldInfo = &(world->getWorldInfo());
	this->world->addSoftBody(softBody);
	this->objects.push_back(object);
	this->softObjects.push_back(object);
}

