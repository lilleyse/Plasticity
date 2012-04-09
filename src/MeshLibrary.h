#pragma once

#include "Mesh.h"
#include <vector>
#include "ColladaData.h"
#include "Loader.h"

class MeshLibrary
{
public:

	MeshLibrary();
	~MeshLibrary();

	void initialize();

	std::vector<Mesh*> meshes;

	Mesh* getMesh(int number);
};
