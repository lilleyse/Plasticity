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

	BaseMesh(std::vector<float>& positionData, std::vector<float>& normalsData, std::vector<unsigned short>& elementArray, bool initializeNeighbors = false);

	std::vector<Vertex> vertices;
	std::vector<unsigned short> elementArray;
	std::vector<int> elementArrayForBullet;
	std::map<int,std::vector<int>> elementNeighbors;

	std::vector<int>& getNeighbors(int index);

protected:
	void initializeNeighbors();
};