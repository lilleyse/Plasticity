#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <bullet/btBulletDynamicsCommon.h>

namespace Utils
{
	glm::mat4 convertBulletTransformToGLM(btTransform& transform);
	btTransform convertGLMTransformToBullet(glm::mat4 transform);
	
	btVector3 convertGLMVectorToBullet(glm::vec3 vector);
	glm::vec3 convertBulletVectorToGLM(btVector3& vector);
}
