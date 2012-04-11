#pragma once
#include <gl3w/gl3w.h>
#include <glm/glm.hpp>
#include <vector>
#include "BaseMesh.h"

enum Attributes
{
	POSITION,
	NORMAL
};


class Mesh
{
public:
	Mesh(BaseMesh* baseMesh);
	~Mesh();

	void render();

	Vertex* getVertices();
	int* getElements();

	void updateVertices(); //updates the vbo
	void updateNormal(int index);
	void updateNeighborNormals(int index);

private:

	int numElements;
	int numVertices;
	GLuint vertexArrayObject;
	GLuint arrayBufferObject;
	GLuint elementBufferObject;

	BaseMesh* baseMesh;
	Vertex* vertices;
};