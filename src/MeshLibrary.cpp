#include "MeshLibrary.h"

MeshLibrary::MeshLibrary()
{

}

MeshLibrary::~MeshLibrary()
{

}

void MeshLibrary::initialize()
{
	std::string filename = "data/meshes/pumpkin.dae";
	ColladaData* data = Loader::readColladaAsset(Utils::getFilePath(filename));
	Mesh* mesh = new Mesh(data->positionData, data->normalsData, data->elementArray);
	meshes.push_back(mesh);
}

Mesh* MeshLibrary::getMesh(int number)
{
	return meshes[number];
}