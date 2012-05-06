#include "Loader.h"

Loader::Loader()
{

}

std::vector<float> Loader::parseDataIntoFloats(const std::string& configData, size_t size)
{

	std::vector<float> floats;
    floats.reserve(size);

	typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
	boost::char_separator<char> sep(" ");
	tokenizer tokens(configData, sep);
	for (tokenizer::iterator tok_iter = tokens.begin(); tok_iter != tokens.end(); ++tok_iter)
	{
		floats.push_back((float)std::atof((*tok_iter).c_str()));
	}

	return floats;
}

std::vector<unsigned short> Loader::parseDataIntoUShorts(const std::string& configData, size_t size)
{
    std::vector<unsigned short> ushorts;
    ushorts.reserve(size);
    
	typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
	boost::char_separator<char> sep(" ");
	tokenizer tokens(configData, sep);
	for (tokenizer::iterator tok_iter = tokens.begin(); tok_iter != tokens.end(); ++tok_iter)
	{
		ushorts.push_back((unsigned short)std::atoi((*tok_iter).c_str()));
	}   

    return ushorts;
}


glm::mat4 Loader::parseDataIntoMat4(const std::string& configData)
{
	//its ROW ORDER. Meaning the first 4 floats are the first row, not the first column
	std::vector<float> floats = parseDataIntoFloats(configData, 16);
	
	glm::mat4 matrix(0);
	matrix[0].x = floats[0];
	matrix[1].x = floats[1];
	matrix[2].x = floats[2];
	matrix[3].x = floats[3];

	matrix[0].y = floats[4];
	matrix[1].y = floats[5];
	matrix[2].y = floats[6];
	matrix[3].y = floats[7];

	matrix[0].z = floats[8];
	matrix[1].z = floats[9];
	matrix[2].z = floats[10];
	matrix[3].z = floats[11];

	matrix[0].w = floats[12];
	matrix[1].w = floats[13];
	matrix[2].w = floats[14];
	matrix[3].w = floats[15];

    return matrix;
}

std::vector<glm::vec3> Loader::parseTranslationData(const std::string& configData, int numTranslations)
{
	//its ROW ORDER. Meaning the first 4 floats are the first row, not the first column
	std::vector<glm::vec3> translationValues;
    translationValues.reserve(numTranslations);
	std::vector<float> floats = parseDataIntoFloats(configData, numTranslations*3);
	for(int i = 0; i < numTranslations; i++)
	{
		float x = floats[i*3 + 0];
		float y = floats[i*3 + 1];
		float z = floats[i*3 + 2];
		translationValues.push_back(glm::vec3(x,y,z));
	}
   
    return translationValues;
}

std::vector<glm::fquat> Loader::parseQuatData(const std::string& configData, int numQuats)
{
	//its ROW ORDER. Meaning the first 4 floats are the first row, not the first column
	std::vector<glm::fquat> quatValues;
    quatValues.reserve(numQuats);
	std::vector<float> floats = parseDataIntoFloats(configData, numQuats*4);
	for(int i = 0; i < numQuats; i++)
	{
		float x = floats[i*3 + 0];
		float y = floats[i*3 + 1];
		float z = floats[i*3 + 2];
		float w = floats[i*3 + 3];
		quatValues.push_back(glm::fquat(x,y,z,w));
	}
   
    return quatValues;
}

Vertex* Loader::loadVertexDataForStaticMesh(TiXmlElement* vertexData, int& numVertices, bool& containsPositions, bool& containsNormals, bool& containsUVs)
{
	std::string vertexCountString = vertexData->Attribute("count");
	numVertices = atoi(vertexCountString.c_str());

	TiXmlElement* positionElement = vertexData->FirstChildElement("position");
	std::string positionData = positionElement->FirstChild()->Value();
	if(positionData == "") containsPositions = false; else containsPositions = true;
	float width = (float)atof(positionElement->Attribute("width"));
	float height = (float)atof(positionElement->Attribute("height"));
	float depth = (float)atof(positionElement->Attribute("depth"));
	
	std::string normalData = vertexData->FirstChildElement("normal")->FirstChild()->Value();
	if(normalData == "") containsNormals = false; else containsNormals = true;
	std::string UVData = "";//vertexData->FirstChildElement("UV")->FirstChild()->Value();
	if(UVData == "") containsUVs = false; else containsUVs = true;
   
	typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
	boost::char_separator<char> sep(" ");
	tokenizer positionTokens(positionData, sep);
	tokenizer normalsTokens(normalData, sep);
	tokenizer UVTokens(UVData, sep);

	tokenizer::iterator positionsIter = positionTokens.begin();
	tokenizer::iterator normalsIter = normalsTokens.begin();
	tokenizer::iterator UVIter = UVTokens.begin();

	Vertex* vertices = new Vertex[numVertices];

	for(int i = 0; i < numVertices; i++)
	{
		if(containsPositions)
		{
			vertices[i].x = (float)std::atof((*(positionsIter++)).c_str());
			vertices[i].y = (float)std::atof((*(positionsIter++)).c_str());
			vertices[i].z = (float)std::atof((*(positionsIter++)).c_str());
		}

		if(containsNormals)
		{
			vertices[i].nx = (float)std::atof((*(normalsIter++)).c_str());
			vertices[i].ny = (float)std::atof((*(normalsIter++)).c_str());
			vertices[i].nz = (float)std::atof((*(normalsIter++)).c_str());
		}

		if(containsUVs)
		{
			vertices[i].s = (float)std::atof((*(UVIter++)).c_str());
			vertices[i].t = (float)std::atof((*(UVIter++)).c_str());
		}
	}   

	return vertices;

}

BaseMesh* Loader::readColladaAsset(std::string& fileName)
{
	std::string fullFilepath = Utils::getFilePath(fileName);
	TiXmlDocument doc;
	doc.LoadFile(fullFilepath.c_str());
	if(doc.Error())
		throw std::runtime_error(doc.ErrorDesc());
	TiXmlHandle docHandle(&doc);

	TiXmlElement* header = docHandle.FirstChildElement("Mesh").ToElement();

	std::string headerType = header->Attribute("type");

	TiXmlElement* elementArrayElement = header->FirstChildElement("element_array");
	int numElements = atoi(elementArrayElement->Attribute("count"));
	std::vector<unsigned short> elementArray = parseDataIntoUShorts(elementArrayElement->FirstChild()->Value(), numElements);

	//the texture name may be ""
	std::string textureName = "";//header->FirstChildElement("texture_name")->FirstChild()->Value();

	TiXmlElement* vertexDataElement = header->FirstChildElement("vertex_data");
	int numVertices;
	bool containsPositions;
	bool containsNormals;
	bool containsUVs;



	if(headerType == "static")
	{
		Vertex* vertexData = loadVertexDataForStaticMesh(vertexDataElement, numVertices, containsPositions, containsNormals, containsUVs);
		BaseMesh* baseMesh = new BaseMesh(numVertices, vertexData, containsPositions, containsNormals, containsUVs, textureName, elementArray);    
		return baseMesh;
	}

	//maybe not needed
	//std::string name = docHandle.FirstChild("info").ToElement()->FirstChildElement("name")->FirstChild()->Value();

	return 0;

}
