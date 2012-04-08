#include "Mesh.h"

Mesh::Mesh(std::vector<float>& positionData, std::vector<float>& normalsData, std::vector<unsigned short>& elementArray)
{
	
	//create opengl mesh based on the positions
	
	int numVertices = positionData.size()/3;
	numElements = elementArray.size();

	Vertex* vertices = new Vertex[numVertices];

	for(int i = 0; i < numVertices; i++)
	{
		vertices[i].x = positionData[i*3 + 0];
		vertices[i].y = positionData[i*3 + 1];
		vertices[i].z = positionData[i*3 + 2];

		vertices[i].nx = normalsData[i*3 + 0];
		vertices[i].ny = normalsData[i*3 + 1];
		vertices[i].nz = normalsData[i*3 + 2];
	}

	//create and bind array buffer, set data
    glGenBuffers(1, &arrayBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, arrayBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*numVertices, vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //create and bind element array buffer, set data to the stored element array, then close buffer
    glGenBuffers(1, &elementBufferObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort)*numElements, &elementArray[0], GL_STATIC_DRAW);
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
	delete[] vertices;

}

Mesh::~Mesh()
{

}

void Mesh::render()
{
	glBindVertexArray(vertexArrayObject);
    glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_SHORT, 0);
    glBindVertexArray(0);
}