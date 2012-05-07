#pragma once

#include <vector>
#include <map>
#include <iostream>
#include <gli/gli.hpp>
#include <gli/gtx/fetch.hpp>
#include <gli/gtx/gradient.hpp>
#include <gli/gtx/loader.hpp>
#include <gl3w/gl3w.h>

struct Vertex
{
	float x, y, z;
	float nx, ny, nz;
	float s, t;
};

struct MaterialBlock
{
	glm::vec4 diffuseColor;
	glm::vec4 specularColor;
	float specularShininess;
	int getColorFromTexture;
	float padding[2];
};

class BaseMesh
{
public:

	BaseMesh(unsigned int numVertices, Vertex* vertexData, bool containsPositions, bool containsNormals, bool containsUVs, std::string& textureFileName, std::vector<unsigned short>& indexArray);
	
	void loadTexture(std::string& textureFileName);
	GLuint mainTexture;

	void initMaterial();
	MaterialBlock material;

	Vertex* vertices;
	int numVertices;
	std::vector<unsigned short> elementArray;
	std::vector<int> elementArrayForBullet;

	bool containsPositions;
	bool containsNormals; 
	bool containsUVs;
};