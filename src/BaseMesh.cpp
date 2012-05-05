#include "BaseMesh.h"


BaseMesh::BaseMesh(std::vector<float>& positionData, std::vector<float>& normalsData, std::vector<unsigned short>& elementArray, bool initializeNeighbors)
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

	//convert unsigned short array to int array
	this->elementArrayForBullet = std::vector<int>(elementArray.size());
	for(unsigned int i = 0; i < elementArray.size(); i++)
	{
		elementArrayForBullet[i] = (int)elementArray[i];
	}

	if(initializeNeighbors)
		this->initializeNeighbors();
}



void BaseMesh::initializeNeighbors()
{
	/*int largestIndex = 0;
	for (unsigned int i = 0; i < elementArrayForBullet.size(); i++)
	{
		int currentIndex = elementArrayForBullet.at(i);
		if(currentIndex > largestIndex)
			largestIndex = currentIndex;
	}
	for(int i = 0; i < largestIndex; i++)
	{
		int currentIndex = i;
		for(unsigned int j = 0; j < elementArrayForBullet.size(); j+=3)
		{
			bool inSameTriangle = false;
			for(unsigned int k = 0; k < 3; k++)
			{
				int otherIndex = elementArrayForBullet.at(j+k);
				if(otherIndex == currentIndex)
				{
					inSameTriangle = true;
					break;
				}
			}
			if(inSameTriangle)
			{
				for(unsigned int k = 0; k < 3; k++)
				{
					int otherIndex = elementArrayForBullet.at(j+k);
					if(otherIndex != currentIndex)
					{
						elementNeighbors[currentIndex].push_back(otherIndex);
					}
				}
			}
		}
	}*/
}

std::vector<int>& BaseMesh::getNeighbors(int index)
{
	return this->elementNeighbors.at(index);
}