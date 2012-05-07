#version 330
#extension GL_ARB_shading_language_420pack : enable


/*-------------------------
		Uniforms
---------------------------*/

layout(binding = 0) uniform Projection
{
	mat4 projectionMatrix;
};

struct ModelViewStruct
{
	mat4 normalMatrix;
	mat4 matrix;
};

layout(binding = 1) uniform ModelView
{
	ModelViewStruct modelView;
};


/*-------------------------
		Inputs
---------------------------*/
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texcoord;

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
	vec2 texcoord;
	
} vertexOut;

/*-------------------------
		Main
---------------------------*/

void main()
{
	vec4 tempCamPosition = (modelView.matrix * vec4(position, 1.0));
	gl_Position = projectionMatrix * tempCamPosition;

	vertexOut.vertexNormal = normalize(mat3(modelView.normalMatrix) * normal);
	vertexOut.cameraSpacePosition = vec3(tempCamPosition);
	vertexOut.texcoord = texcoord;
}
