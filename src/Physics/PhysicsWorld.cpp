#include "PhysicsWorld.h"

PhysicsWorld::PhysicsWorld()
{
	//Set up bullet objects
	this->collisionConfiguration = new btDefaultCollisionConfiguration();
	this->dispatcher = new btCollisionDispatcher(this->collisionConfiguration);
	this->broadphase = new btDbvtBroadphase();
	this->solver = new btSequentialImpulseConstraintSolver();
	
	//Set up the world
	this->dynamicsWorld = new btDiscreteDynamicsWorld(this->dispatcher,this->broadphase,this->solver,this->collisionConfiguration);
	this->dynamicsWorld->getSolverInfo().m_splitImpulse = true;
	this->dynamicsWorld->getSolverInfo().m_numIterations = 20;
	this->dynamicsWorld->getDispatchInfo().m_useContinuous = true;
	this->dynamicsWorld->setGravity(btVector3(0,-10,0));
}
PhysicsWorld::~PhysicsWorld(){}

void PhysicsWorld::update()
{
	this->dynamicsWorld->stepSimulation(1.0f/60.0f);
	this->processCollisions();
}
void PhysicsWorld::processCollisions()
{
	int numManifolds = this->dynamicsWorld->getDispatcher()->getNumManifolds();
	for (int i=0;i<numManifolds;i++)
	{
		btPersistentManifold* contactManifold =  this->dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
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
	if(ob->getCollisionShape()->isConcave())
	{
		float impulse = pt.m_appliedImpulse;
		float distance = pt.getDistance();
		if (distance < 0.f && impulse > 5.0f)
		{
			glm::vec3 intersectionPos = Utils::convertBulletVectorToGLM(pos);

			std::cout << "Impulse: " << impulse << std::endl;			
			std::cout << "normal: ";
			Utils::printVec3(Utils::convertBulletVectorToGLM(normal));

			btCollisionShape* trimesh = ob->getCollisionShape();
			Mesh* renderMesh = (Mesh*)trimesh->getUserPointer();
			Vertex* vertices = renderMesh->getVertices();
			int* elements = renderMesh->getElements();

			//Find the closest index in the intersection triangle
			int bestIndex;
			float bestDistance = 10000;
			for(int k = 0; k < 3; k++)
			{
				int index = elements[triIndex*3+k];
				Vertex& vertexInTriangle = vertices[index];
				glm::vec3 vertexPos = glm::vec3(vertexInTriangle.x, vertexInTriangle.y, vertexInTriangle.z);
				float distanceFromIntersectionPoint = glm::distance(intersectionPos, vertexPos);
				if(distanceFromIntersectionPoint < bestDistance)
				{
					bestDistance = distanceFromIntersectionPoint;
					bestIndex = index;
				}
			}

			//Update the positions for all neighbors around the intersection point
			float range = 1.5f;
			float magnitude = -impulse*.05f;
			std::vector<int> neighbors = renderMesh->getBaseMesh()->getNeighbors(bestIndex);
			neighbors.push_back(bestIndex);

			for(unsigned int k = 0; k < neighbors.size(); k+=2)
			{
				int neighborIndex = neighbors[k];
				Vertex& neighbor = vertices[neighborIndex];
				glm::vec3 neighborPos = glm::vec3(neighbor.x,neighbor.y,neighbor.z);
				float distanceFromIntersectionPoint = glm::distance(intersectionPos, neighborPos);
				float clampedDistance = glm::clamp((range - distanceFromIntersectionPoint)/range, 0.0f, 1.0f);
				neighbor.x += normal.getX()*magnitude*clampedDistance;
				neighbor.y += normal.getY()*magnitude*clampedDistance;
				neighbor.z += normal.getZ()*magnitude*clampedDistance;
			}

			for(unsigned int k = 0; k < neighbors.size(); k+=2)
			{
				int neighborIndex = neighbors[k];
				renderMesh->updateNormal(neighborIndex);
				renderMesh->updateNeighborNormals(neighborIndex);
			}

			//obB->setDamping(100.0,100.0);
			//obB->applyDamping(1/60.0);
			//btVector3 oppositeForce = btVector3(1000,1000,1000);
			//btTransform bulletTransform = obB->getWorldTransform();
			//btVector3 position = btVector3(0,0,0);
			//bulletTransform.setOrigin(position);
			//obB->setWorldTransform(bulletTransform);

			renderMesh->updateVertices();
			//Clean the intersections
			//trimesh->postUpdate();
			//trimeshe->partialRefitTree(aabbMin,aabbMax);
			//this->dynamicsWorld->getBroadphase()->getOverlappingPairCache()->cleanProxyFromPairs(obA->getBroadphaseHandle(),this->dynamicsWorld->getDispatcher());
		}
	}
}
void PhysicsWorld::addObject(PhysicsObject* object)
{
	this->dynamicsWorld->addRigidBody(object->getRigidBody());
	this->objects.push_back(object);
}
std::vector<PhysicsObject*>& PhysicsWorld::getObjects()
{
	return this->objects;
}

