#version 330
#extension GL_ARB_shading_language_420pack : enable

#define MAX_LIGHTS 6

/*-------------------------
		Uniforms
---------------------------*/

layout(binding = 0) uniform sampler2D diffuseColorSampler;

struct PerLight
{
	vec4 cameraSpaceLightPos;
	vec4 lightIntensity;
	float lightAttenuation;
};

layout(binding = 3) uniform NumLightsUsed
{
	int numLightsUsed;
};

layout(binding = 4) uniform Light
{
	vec4 ambientIntensity;
	float maxIntensity;
	float gamma;
	PerLight lights[MAX_LIGHTS];
} Lgt;

layout(binding = 5) uniform Material
{
	vec4 diffuseColor;
	vec4 specularColor;
	float specularShininess;
	int getColorFromTexture;
} Mtl;


/*-------------------------
		Inputs
---------------------------*/

in Vertex
{
	vec3 vertexNormal;
	vec3 cameraSpacePosition;
	vec2 texcoord;
	
} vertexIn;

/*-------------------------
		Outputs
---------------------------*/

layout (location = 0, index = 0) out vec4 fragColor;


/*---------------------------
		Global vars
----------------------------*/

vec4 diffuseColor;

/*-------------------------
		Functions
---------------------------*/

float CalcAttenuation(in vec3 cameraSpacePosition,
	in vec3 cameraSpaceLightPos,
	in float lightAttenuation,
	out vec3 lightDirection)
{
	vec3 lightDifference =  cameraSpaceLightPos - vertexIn.cameraSpacePosition;
	float lightDistanceSqr = dot(lightDifference, lightDifference);
	lightDirection = lightDifference * inversesqrt(lightDistanceSqr);
	
	return (1 / ( 1.0 + lightAttenuation * lightDistanceSqr));
}

vec4 ComputeLighting(in PerLight lightData)
{
	vec3 lightDir;
	vec4 lightIntensity;

	float atten = CalcAttenuation(vertexIn.cameraSpacePosition, lightData.cameraSpaceLightPos.xyz, lightData.lightAttenuation, lightDir);
	lightIntensity = atten * lightData.lightIntensity;
	
	//make all normals point front facing for transparency
	vec3 surfaceNormal = normalize(vertexIn.vertexNormal);
	float cosAngIncidence = dot(surfaceNormal, lightDir);
	cosAngIncidence = cosAngIncidence < 0.0001 ? 0.0 : cosAngIncidence;
	
	vec3 viewDirection = normalize(-vertexIn.cameraSpacePosition);
	
	vec3 halfAngle = normalize(lightDir + viewDirection);
	float angleNormalHalf = acos(dot(halfAngle, surfaceNormal));
	float exponent = angleNormalHalf / Mtl.specularShininess;
	exponent = -(exponent * exponent);
	float gaussianTerm = exp(exponent);

	gaussianTerm = cosAngIncidence != 0.0 ? gaussianTerm : 0.0;
	
	vec4 lighting = diffuseColor * lightIntensity * cosAngIncidence;
	lighting += Mtl.specularColor * lightIntensity * gaussianTerm;
	
	return lighting;
}

/*-------------------------
		Main
---------------------------*/

void main()
{

	//fragColor = vec4(vertexIn.texcoord.x, vertexIn.texcoord.y, 0, 1);
	if(Mtl.getColorFromTexture == 0)
	{
		diffuseColor = Mtl.diffuseColor;
	}
	else
	{
		diffuseColor = texture(diffuseColorSampler, vertexIn.texcoord);
	}



	vec4 accumLighting = diffuseColor*Lgt.ambientIntensity;
	for(int light = 0; light < numLightsUsed; light++)
	{
		accumLighting += ComputeLighting(Lgt.lights[light]);
	}
	
	accumLighting = accumLighting / Lgt.maxIntensity;
	vec4 gamma = vec4(1.0 / Lgt.gamma);
	gamma.w = 1.0;
	fragColor = pow(accumLighting, gamma);

}
