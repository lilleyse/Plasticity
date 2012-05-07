#version 420

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texcoord;


out Vertex
{
	vec3 modelSpaceNormal;
	vec3 modelSpacePosition;
	vec2 texcoord;
} vertexOut;


void main()
{
	vertexOut.modelSpacePosition = position;
	vertexOut.modelSpaceNormal = normal;
	vertexOut.texcoord = texcoord;
}
