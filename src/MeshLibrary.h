#pragma once

#include "BaseMesh.h"
#include <vector>
#include "ColladaData.h"
#include "Loader.h"

class MeshLibrary
{
public:

	MeshLibrary();
	~MeshLibrary();

	void initialize();

	std::vector<BaseMesh*> meshes;

	BaseMesh* getMesh(int number);
};
