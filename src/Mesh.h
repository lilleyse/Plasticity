#pragma once
#include <gl3w/gl3w.h>
#include <vector>

struct Vertex
{
	float x, y, z;
	float nx, ny, nz;
	float s, t;
};

enum Attributes
{
	POSITION,
	NORMAL
};


class Mesh
{
public:
	Mesh(std::vector<float>& positionData, std::vector<float>& normalsData, std::vector<unsigned short>& elementArray);
	~Mesh();

	void render();

private:

	int numElements;
	GLuint vertexArrayObject;
	GLuint arrayBufferObject;
	GLuint elementBufferObject;


};