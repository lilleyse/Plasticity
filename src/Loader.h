#pragma once

#include <tinyxml/tinyxml.h>
#include "BaseMesh.h"
#include "Utils.h"
#include <boost/tokenizer.hpp>
#include "glm/gtx/quaternion.hpp"


class Loader
{
public:
	Loader();
	BaseMesh* readColladaAsset(std::string& fileName);

private:
	Vertex* loadVertexDataForStaticMesh(TiXmlElement* vertexData, int& numVertices, bool& containsPositions, bool& containsNormals, bool& containsUVs);
	std::vector<float> parseDataIntoFloats(const std::string& configData, size_t size);
	std::vector<unsigned short> parseDataIntoUShorts(const std::string& configData, size_t size);
	glm::mat4 parseDataIntoMat4(const std::string& configData);
	std::vector<glm::vec3> Loader::parseTranslationData(const std::string& configData, int numTranslations);
	std::vector<glm::fquat> Loader::parseQuatData(const std::string& configData, int numQuats);

};



