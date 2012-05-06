#include "BaseMesh.h"


BaseMesh::BaseMesh(unsigned int numVertices, Vertex* vertexData, bool containsPositions, bool containsNormals, bool containsUVs, std::string& textureFileName, std::vector<unsigned short>& indexArray)
{	
	this->vertices = vertexData;
	this->numVertices = numVertices;
	this->elementArray = indexArray;

	//convert unsigned short array to int array
	this->elementArrayForBullet = std::vector<int>(elementArray.size());
	for(unsigned int i = 0; i < elementArray.size(); i++)
	{
		elementArrayForBullet[i] = (int)elementArray[i];
	}
}
