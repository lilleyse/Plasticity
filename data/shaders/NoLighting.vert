#version 330
#extension GL_ARB_shading_language_420pack : enable


/*-------------------------
		Uniforms
---------------------------*/

layout(binding = 0) uniform Projection
{
	mat4 projectionMatrix;
};

layout(binding = 1) uniform ModelView
{
	mat4 modelViewMatrix;
};


/*-------------------------
		Inputs
---------------------------*/
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

/*-------------------------
		Outputs
---------------------------*/

out gl_PerVertex
{
	vec4 gl_Position;
	float gl_PointSize;
	float gl_ClipDistance[];
};

out Vertex
{
	vec3 vertexNormal;
	vec3 cameraSpacePosition;
	
} vertexOut;

/*-------------------------
		Main
---------------------------*/

void main()
{
	vec4 tempCamPosition = (modelViewMatrix * vec4(position, 1.0));
	gl_Position = projectionMatrix * tempCamPosition;

	vertexOut.vertexNormal = normalize(mat3(modelViewMatrix) * normal);
	vertexOut.cameraSpacePosition = vec3(tempCamPosition);
}
