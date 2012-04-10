#pragma once
#include <gl3w/gl3w.h>
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
	Mesh(BaseMesh& baseMesh);
	~Mesh();

	void render();

private:

	int numElements;
	GLuint vertexArrayObject;
	GLuint arrayBufferObject;
	GLuint elementBufferObject;


};