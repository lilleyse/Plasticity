#pragma once
#include <gl3w/gl3w.h>
#include <glm/glm.hpp>
#include <vector>
#include "BaseMesh.h"

enum Attributes
{
	POSITION,
	NORMAL,
	UV
};



struct NormalHelper
{
	glm::vec3 normal;
	void updateNormal(glm::vec3& orig, glm::vec3& p1, glm::vec3& p2);
};

class Mesh
{
public:
	Mesh(BaseMesh* baseMesh, GLenum usage);
	~Mesh();

	void render();

	Vertex* getVertices();
	int* getElements();
	BaseMesh* getBaseMesh();

	void updateVertices(); //updates the vbo
	void updateNormals();

	int numElements;
	int numVertices;

private:
	GLuint vertexArrayObject;
	GLuint arrayBufferObject;
	GLuint elementBufferObject;

	BaseMesh* baseMesh;
	Vertex* vertices;
	
	NormalHelper* normalHelperArray;
};