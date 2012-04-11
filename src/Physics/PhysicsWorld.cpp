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

	int numManifolds = this->dynamicsWorld->getDispatcher()->getNumManifolds();
	for (int i=0;i<numManifolds;i++)
	{
		btPersistentManifold* contactManifold =  this->dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
		btCollisionObject* obA = static_cast<btCollisionObject*>(contactManifold->getBody0());
		btCollisionObject* obB = static_cast<btCollisionObject*>(contactManifold->getBody1());
		
		if(obB->getCollisionShape()->isConcave())
			obA = obB;

		int numContacts = contactManifold->getNumContacts();
		for (int j=0;j<numContacts;j++)
		{
			btManifoldPoint& pt = contactManifold->getContactPoint(j);
			float impulse = pt.getAppliedImpulse();
			if (pt.getDistance() < 0.f && impulse > 1.0f)
			{
				const btVector3& ptA = pt.getPositionWorldOnA();
				const btVector3& ptB = pt.getPositionWorldOnB();

				std::cout << "Impulse: " << impulse << std::endl;
				if(obA->getCollisionShape()->isConcave())
				{
					int indexA = pt.m_index0;
					btVector3 normalOnA = -pt.m_normalWorldOnB;
					std::cout << "A: ";
					std::cout << "normal: ";
					Utils::printVec3(Utils::convertBulletVectorToGLM(normalOnA));
					std::cout << "index: " << indexA << std::endl;

					//Update vertex
					btTriangleIndexVertexArray* triArray = 0;
					btCollisionShape* trimesh = obA->getCollisionShape();
					if(dynamic_cast<btGImpactMeshShape*>(trimesh) != 0)
						triArray = (btTriangleIndexVertexArray*)(((btGImpactMeshShape*)trimesh)->getMeshInterface());
					if(dynamic_cast<btBvhTriangleMeshShape*>(trimesh) != 0)
						triArray = (btTriangleIndexVertexArray*)(((btBvhTriangleMeshShape*)trimesh)->getMeshInterface());
					
					int triangle[3];
					for(int i = 0; i < 3; i++)
					{
						triArray->getIndexedMeshArray()[0];
						triangle[i] = ((int*)triArray->getIndexedMeshArray()[0].m_triangleIndexBase)[indexA*3+i];
					}

					Vertex* vertexData = ((Vertex*)triArray->getIndexedMeshArray()[0].m_vertexBase);
					for(int i = 0; i < 3; i++)
					{
						int index = triangle[i];
						//std::cout << "vertex: ";
						//Utils::printVec3(glm::vec3(vertexData[triangle[2]].x, vertexData[triangle[2]].y, vertexData[triangle[2]].z));
						
						float magnitude = -0.5f;
						vertexData[index].x += normalOnA.getX()*magnitude;
						vertexData[index].y += normalOnA.getY()*magnitude;
						vertexData[index].z += normalOnA.getZ()*magnitude;
					}

					Mesh* renderMesh = (Mesh*)trimesh->getUserPointer();
					renderMesh->updateVertices(vertexData);

					//Clean the intersections
					//trimesh->postUpdate();
					//trimeshe->partialRefitTree(aabbMin,aabbMax);
					//this->dynamicsWorld->getBroadphase()->getOverlappingPairCache()->cleanProxyFromPairs(obA->getBroadphaseHandle(),this->dynamicsWorld->getDispatcher());
				}
			}
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

