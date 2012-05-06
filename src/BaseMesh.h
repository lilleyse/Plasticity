#pragma once

#include <vector>
#include <map>
#include <iostream>

struct Vertex
{
	float x, y, z;
	float nx, ny, nz;
	float s, t;
};

class BaseMesh
{
public:

	BaseMesh(unsigned int numVertices, Vertex* vertexData, bool containsPositions, bool containsNormals, bool containsUVs, std::string& textureFileName, std::vector<unsigned short>& indexArray);


	Vertex* vertices;
	int numVertices;
	std::vector<unsigned short> elementArray;
	std::vector<int> elementArrayForBullet;
};