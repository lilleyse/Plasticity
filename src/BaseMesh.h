#pragma once

#include <vector>

struct Vertex
{
	float x, y, z;
	float nx, ny, nz;
	float s, t;
};

class BaseMesh
{
public:

	BaseMesh(std::vector<float>& positionData, std::vector<float>& normalsData, std::vector<unsigned short>& elementArray);

	std::vector<Vertex> vertices;
	std::vector<unsigned short> elementArray;

};