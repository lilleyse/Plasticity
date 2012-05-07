#include "Mesh.h"

Mesh::Mesh(BaseMesh* baseMesh, GLenum usage)
{

	numVertices = baseMesh->numVertices;
	numElements = baseMesh->elementArray.size();

	normalHelperArray = new NormalHelper[numVertices];



	this->baseMesh = baseMesh;
	this->vertices = baseMesh->vertices;

	//create and bind array buffer, set data
    glGenBuffers(1, &arrayBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, arrayBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*numVertices, vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //create and bind element array buffer, set data to the stored element array, then close buffer
    glGenBuffers(1, &elementBufferObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort)*numElements, &baseMesh->elementArray[0], usage);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//create and bind vao
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);

    //bind array buffer again
    glBindBuffer(GL_ARRAY_BUFFER, arrayBufferObject);

	//enable vertex attributes
	if(baseMesh->containsPositions) glEnableVertexAttribArray(GLuint(POSITION));
	if(baseMesh->containsNormals) glEnableVertexAttribArray(GLuint(NORMAL));
	if(baseMesh->containsUVs) glEnableVertexAttribArray(GLuint(UV));

    //set vertex attrib pointers
	size_t offset = 0;
    glBindBuffer(GL_ARRAY_BUFFER, arrayBufferObject);
	
	if(baseMesh->containsPositions)
	{
		glVertexAttribPointer(POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
		offset += sizeof(float)*3;
	}

	if(baseMesh->containsNormals)
	{
		glVertexAttribPointer(NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offset));
		offset += sizeof(float)*3;
	}

	if(baseMesh->containsUVs)
	{
		glVertexAttribPointer(UV, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offset));
		offset += sizeof(float)*2;
	}

	//bind element array
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObject);

	//cleanup
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	
	
}

Mesh::~Mesh(){}

Vertex* Mesh::getVertices()
{
	return this->vertices;
}
int* Mesh::getElements()
{
	return &this->baseMesh->elementArrayForBullet[0];
}

BaseMesh* Mesh::getBaseMesh()
{
	return this->baseMesh;
}

void Mesh::updateVertices()
{
	glBindBuffer(GL_ARRAY_BUFFER, arrayBufferObject);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex)*numVertices, vertices);
}

void Mesh::render()
{

	//set texture for render
	if(baseMesh->containsUVs)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, baseMesh->mainTexture);
	}

	glBindVertexArray(vertexArrayObject);
    glDrawElements(GL_PATCHES, numElements, GL_UNSIGNED_SHORT, 0);
    glBindVertexArray(0);
}


void NormalHelper::updateNormal(glm::vec3& orig, glm::vec3& p1, glm::vec3& p2)
{
	glm::vec3 edge1 = glm::normalize(orig - p1);
	glm::vec3 edge2 = glm::normalize(orig - p2);
	glm::vec3 cross = glm::cross(edge1,edge2);
	normal += cross;
}

void Mesh::updateNormals()
{
	for(int i = 0; i < numElements; i+=3)
	{
		int index0 = baseMesh->elementArrayForBullet[i+0];
		int index1 = baseMesh->elementArrayForBullet[i+1];
		int index2 = baseMesh->elementArrayForBullet[i+2];

		glm::vec3 vertex0 = glm::vec3(vertices[index0].x, vertices[index0].y, vertices[index0].z);
		glm::vec3 vertex1 = glm::vec3(vertices[index1].x, vertices[index1].y, vertices[index1].z);
		glm::vec3 vertex2 = glm::vec3(vertices[index2].x, vertices[index2].y, vertices[index2].z);

		normalHelperArray[index0].updateNormal(vertex0, vertex1, vertex2);
		normalHelperArray[index1].updateNormal(vertex1, vertex2, vertex0);
		normalHelperArray[index2].updateNormal(vertex2, vertex0, vertex1);
	}

	for(int i = 0; i < numVertices; i++)
	{
		glm::vec3 normal = glm::normalize(normalHelperArray[i].normal);
		normalHelperArray[i].normal = glm::vec3(0,0,0);
		vertices[i].nx = normal.x;
		vertices[i].ny = normal.y;
		vertices[i].nz = normal.z;
	}
}
