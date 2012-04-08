#pragma once
#include <gl3w/gl3w.h>
#include <dae.h>
#include <dom/domCOLLADA.h>

class ColladaData
{

public:

	unsigned int numVertices;
	std::vector<GLfloat> positionData;
	std::vector<GLfloat> normalsData;
	std::vector<GLfloat> textureData;
	std::vector<GLushort> elementArray;
	GLenum primitiveType;
	float width;
	float height;
	float depth;
	std::string textureFileName;
};