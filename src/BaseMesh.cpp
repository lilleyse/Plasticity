#include "BaseMesh.h"


BaseMesh::BaseMesh(unsigned int numVertices, Vertex* vertexData, bool containsPositions, bool containsNormals, bool containsUVs, std::string& textureFileName, std::vector<unsigned short>& indexArray)
{	
	this->vertices = vertexData;
	this->numVertices = numVertices;
	this->elementArray = indexArray;

	this->containsPositions = containsPositions;
	this->containsNormals = containsNormals;
	this->containsUVs = containsUVs;

	//convert unsigned short array to int array
	this->elementArrayForBullet = std::vector<int>(elementArray.size());
	for(unsigned int i = 0; i < elementArray.size(); i++)
	{
		elementArrayForBullet[i] = (int)elementArray[i];
	}

	if(containsUVs && textureFileName != "")
	{
		loadTexture(textureFileName);
	}

	initMaterial();
}

void BaseMesh::loadTexture(std::string& textureFileName)
{	
	/*------------------------------
		load the main RGBA texture
	---------------------------------*/
	gli::texture2D loadedSkinTexture = gli::load("data/images/" + textureFileName);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &mainTexture);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mainTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, GLint(loadedSkinTexture.levels()-1));
	
	GLenum glFormatSource;
	GLenum glFormatInternal;
	gli::format textureFormat = loadedSkinTexture.format();
	if(textureFormat == gli::RGB8U)
	{
		glFormatSource = GL_RGB;
		glFormatInternal = GL_RGB8;
	}

	glTexStorage2D(GL_TEXTURE_2D, GLint(loadedSkinTexture.levels()), glFormatInternal, GLsizei(loadedSkinTexture[0].dimensions().x), GLsizei(loadedSkinTexture[0].dimensions().y));

	for(gli::texture2D::level_type Level = 0; Level < loadedSkinTexture.levels(); ++Level)
	{
		glTexSubImage2D(
			GL_TEXTURE_2D, 
			GLint(Level), 
			0, 0, 
			GLsizei(loadedSkinTexture[Level].dimensions().x), 
			GLsizei(loadedSkinTexture[Level].dimensions().y), 
			glFormatSource, GL_UNSIGNED_BYTE, 
			loadedSkinTexture[Level].data());
	}
	
	glBindTexture(GL_TEXTURE_2D, 0);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
}

void BaseMesh::initMaterial()
{
	material.diffuseColor = glm::vec4(1,1,0,1);
	material.specularColor = glm::vec4(1,1,1,1);
	material.specularShininess = 0.1f;
	if(this->containsUVs) material.getColorFromTexture = 1;
	else material.getColorFromTexture = 0;
}