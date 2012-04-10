#include "MeshLibrary.h"

MeshLibrary::MeshLibrary(){}
MeshLibrary::~MeshLibrary(){}

void MeshLibrary::initialize()
{
	ColladaData* data;
	std::string filename;

	filename = "data/meshes/pumpkin.dae";
	data = Loader::readColladaAsset(Utils::getFilePath(filename));
	meshes.push_back(new BaseMesh(data->positionData, data->normalsData, data->elementArray));

	filename = "data/meshes/cube.dae";
	data = Loader::readColladaAsset(Utils::getFilePath(filename));
	meshes.push_back(new BaseMesh(data->positionData, data->normalsData, data->elementArray));

	filename = "data/meshes/cylinder.dae";
	data = Loader::readColladaAsset(Utils::getFilePath(filename));
	meshes.push_back(new BaseMesh(data->positionData, data->normalsData, data->elementArray));

	filename = "data/meshes/sphere.dae";
	data = Loader::readColladaAsset(Utils::getFilePath(filename));
	meshes.push_back(new BaseMesh(data->positionData, data->normalsData, data->elementArray));
}

BaseMesh* MeshLibrary::getMesh(int number)
{
	return meshes[number];
}