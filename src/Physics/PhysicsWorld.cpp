#include "PhysicsWorld.h"

PhysicsWorld::PhysicsWorld()
{
	this->collisionConfiguration = new btSoftBodyRigidBodyCollisionConfiguration();
	this->dispatcher = new btCollisionDispatcher(this->collisionConfiguration);
	this->broadphase = new btDbvtBroadphase();
	this->solver = new btSequentialImpulseConstraintSolver();
	this->world = new btSoftRigidDynamicsWorld(this->dispatcher,this->broadphase,this->solver,this->collisionConfiguration);
	this->world->setGravity(btVector3(0,-10,0));
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

		int numContacts = contactManifold->getNumContacts();
		for (int j=0;j<numContacts;j++)
		{
			btManifoldPoint& pt = contactManifold->getContactPoint(j);
			int indexA = pt.m_index0;
			int indexB = pt.m_index1;
			btVector3 ptA = pt.getPositionWorldOnA();
			btVector3 ptB = pt.getPositionWorldOnB();
			btVector3 normalOnB = pt.m_normalWorldOnB;
			btVector3 normalOnA = -pt.m_normalWorldOnB;
			float impulse = pt.m_appliedImpulse;
			this->processCollision(obA,pt,indexA,ptA,normalOnA);
			this->processCollision(obB,pt,indexB,ptB,normalOnB);
		}
	}
}

void PhysicsWorld::processCollision(btRigidBody* ob, btManifoldPoint& pt, int triIndex, btVector3& pos, btVector3& normal)
{
	float threshhold = .3f;
	float maxMagnitude = 2.0f;
	if(ob->getCollisionShape()->isConcave())
	{
		float impulse = pt.m_appliedImpulse;
		float distance = pt.getDistance();
		if (distance < 0.f && impulse > threshhold)
		{
			glm::vec3 intersectionPos = Utils::convertBulletVectorToGLM(pos);
			btCollisionShape* trimesh = ob->getCollisionShape();
			Mesh* mesh = (Mesh*)trimesh->getUserPointer();
			Vertex* vertices = mesh->getVertices();
			int numVertices = mesh->numVertices;

			//Update the positions for all neighbors around the intersection point
			float range = 5.0f;
			float magnitude = -impulse*.5f;
			if(magnitude < -maxMagnitude) magnitude = -maxMagnitude;

			for(int i = 0; i < numVertices; i++)
			{
				glm::vec3 vertex = glm::vec3(vertices[i].x, vertices[i].y, vertices[i].z);
				float distanceFromIntersection = glm::distance(intersectionPos,vertex);
				float clampedDistance = glm::clamp((range - distanceFromIntersection)/range, 0.0f, 1.0f);
				vertices[i].x += normal.getX()*magnitude*clampedDistance;
				vertices[i].y += normal.getY()*magnitude*clampedDistance;
				vertices[i].z += normal.getZ()*magnitude*clampedDistance;
			}

			mesh->updateNormals();
			mesh->updateVertices();
			//Clean the intersections
			//trimesh->postUpdate();
			//trimeshe->partialRefitTree(aabbMin,aabbMax);
			//this->dynamicsWorld->getBroadphase()->getOverlappingPairCache()->cleanProxyFromPairs(obA->getBroadphaseHandle(),this->dynamicsWorld->getDispatcher());
		}
	}
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

