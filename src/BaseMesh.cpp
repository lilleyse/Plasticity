#include "BaseMesh.h"


BaseMesh::BaseMesh(std::vector<float>& positionData, std::vector<float>& normalsData, std::vector<unsigned short>& elementArray)
{
	int numVertices = positionData.size()/3;

	vertices = std::vector<Vertex>(numVertices);

	for(int i = 0; i < numVertices; i++)
	{
		vertices[i].x = positionData[i*3 + 0];
		vertices[i].y = positionData[i*3 + 1];
		vertices[i].z = positionData[i*3 + 2];

		vertices[i].nx = normalsData[i*3 + 0];
		vertices[i].ny = normalsData[i*3 + 1];
		vertices[i].nz = normalsData[i*3 + 2];
	}

	this->elementArray = elementArray;

}