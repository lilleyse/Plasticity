#include "Mesh.h"

Mesh::Mesh(BaseMesh* baseMesh)
{

	numVertices = baseMesh->vertices.size();
	numElements = baseMesh->elementArray.size();

	this->baseMesh = baseMesh;
	vertices = new Vertex[numVertices];
	memcpy(vertices, &baseMesh->vertices[0], sizeof(Vertex)*numVertices);
	//create opengl mesh based on the positions
	
	

	//create and bind array buffer, set data
    glGenBuffers(1, &arrayBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, arrayBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*numVertices, vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //create and bind element array buffer, set data to the stored element array, then close buffer
    glGenBuffers(1, &elementBufferObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort)*numElements, &baseMesh->elementArray[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//create and bind vao
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);

    //bind array buffer again
    glBindBuffer(GL_ARRAY_BUFFER, arrayBufferObject);

	//enable vertex attributes
	glEnableVertexAttribArray(GLuint(POSITION));
	glEnableVertexAttribArray(GLuint(NORMAL));

    //set vertex attrib pointers
	size_t offset = 0;
    glBindBuffer(GL_ARRAY_BUFFER, arrayBufferObject);
	glVertexAttribPointer(POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	offset = sizeof(float)*3;
	glVertexAttribPointer(NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offset));

	//bind element array
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObject);

	//cleanup
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

Mesh::~Mesh()
{

}

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
	glBindVertexArray(vertexArrayObject);
    glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_SHORT, 0);
    glBindVertexArray(0);
}

void Mesh::updateNormal(int index)
{
	Vertex& vertex = this->vertices[index];
	glm::vec3 position = glm::vec3(vertex.x,vertex.y,vertex.z);
	std::vector<int>& neighbors = this->baseMesh->getNeighbors(index);
	glm::vec3 newNormal = glm::vec3(0,0,0);

	for(unsigned int i = 0; i < neighbors.size(); i+=2)
	{
		Vertex& neighbor1 = this->vertices[neighbors[i]];
		Vertex& neighbor2 = this->vertices[neighbors[i+1]];
		glm::vec3 neighbor1Pos = glm::vec3(neighbor1.x,neighbor1.y,neighbor1.z);
		glm::vec3 neighbor2Pos = glm::vec3(neighbor2.x,neighbor2.y,neighbor2.z);
		glm::vec3 edge1 = glm::normalize(position - neighbor1Pos);
		glm::vec3 edge2 = glm::normalize(position - neighbor2Pos);
		glm::vec3 cross = glm::cross(edge1,edge2);
		newNormal += cross;
	}
	newNormal = glm::normalize(newNormal);
	vertex.nx = newNormal.x;
	vertex.ny = newNormal.y;
	vertex.nz = newNormal.z;
}
void Mesh::updateNeighborNormals(int index)
{
	std::vector<int>& neighbors = this->baseMesh->getNeighbors(index);
	for(unsigned int i = 0; i < neighbors.size(); i++)
		this->updateNormal(neighbors[i]);
}