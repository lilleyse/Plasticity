#include "MeshLibrary.h"

MeshLibrary::MeshLibrary(){}
MeshLibrary::~MeshLibrary(){}

void MeshLibrary::initialize()
{
	Loader loader;

	std::string filename;

	//should be pumpkin
	filename = "data/meshes/cube.xml";
	meshes.push_back(loader.readColladaAsset(Utils::getFilePath(filename)));

	filename = "data/meshes/cube.xml";
	meshes.push_back(loader.readColladaAsset(Utils::getFilePath(filename)));

	filename = "data/meshes/cylinder.xml";
	meshes.push_back(loader.readColladaAsset(Utils::getFilePath(filename)));

	filename = "data/meshes/sphere.xml";
	meshes.push_back(loader.readColladaAsset(Utils::getFilePath(filename)));
	
	filename = "data/meshes/weirdConcave.xml";
	meshes.push_back(loader.readColladaAsset(Utils::getFilePath(filename)));

	filename = "data/meshes/subdividedFloor.xml";
	meshes.push_back(loader.readColladaAsset(Utils::getFilePath(filename)));

	filename = "data/meshes/sphereFloor.xml";
	meshes.push_back(loader.readColladaAsset(Utils::getFilePath(filename)));
}

BaseMesh* MeshLibrary::getMesh(int number)
{
	return meshes[number];
}