#include <gl3w/gl3w.h>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <glm/glm.hpp>
#include "Utils.h"
#include "Globals.h"
#include "Cameras/Camera3rdPerson.h"

namespace
{
	glm::mat4 projectionMatrix;
	float fzNear;
    float fzFar;
    float frustumScale;
	int width;
	int height;

	GLuint projectionUBO;
	GLuint projectionBindingIndex;

	GLuint modelViewUBO;
	GLuint modelViewBindingIndex;

	GLuint lightingUBO;
	GLuint lightingBindingIndex;

	GLuint numLightsUBO;
	GLuint numLightsBindingIndex;

	GLuint materialUBO;
	GLuint materialBindingIndex;

	//material

	struct MaterialBlock
	{
		glm::vec4 diffuseColor;
		glm::vec4 specularColor;
		float specularShininess;
		float padding[3];
	};

	//lighting

	const int MAX_NUMBER_OF_LIGHTS = 8;

	struct GL_Light
	{
		glm::vec4 cameraSpaceLightPos;
		glm::vec4 lightIntensity;
		float lightAttenuation;
		float padding[3];
	};

	struct GL_Lighting
	{
		GL_Lighting()
		{
			ambientIntensity = glm::vec4(0,0,0,0);
			maxIntensity = 1.0f;
			gamma = 2.2f;
		}

		glm::vec4 ambientIntensity;
		float maxIntensity;
		float gamma;
		float padding[2];
		GL_Light lights[MAX_NUMBER_OF_LIGHTS];
	};



	Camera3rdPerson camera;


}

void init()
{
	//init OpenGL
	gl3wInit();
	fzNear = .10f;
    fzFar = 100.0f;
    float fieldOfViewDeg = 45.0f;
	float fFovRad = fieldOfViewDeg * DEGREE_TO_RAD;
    frustumScale = 1.0f / tan(fFovRad / 2.0f);

	projectionMatrix = glm::mat4(0.0f);
    projectionMatrix[0].x = frustumScale;
    projectionMatrix[1].y = frustumScale;
    projectionMatrix[2].z = (fzFar + fzNear) / (fzNear - fzFar);
    projectionMatrix[2].w = -1.0f;
    projectionMatrix[3].z = (2 * fzFar * fzNear) / (fzNear - fzFar);

	glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

	glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);
    glDepthRange(0.0f, 1.0f);

	glGenBuffers(1, &projectionUBO);
    glBindBuffer(GL_UNIFORM_BUFFER, projectionUBO);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
	projectionBindingIndex = 0;
    glBindBufferBase(GL_UNIFORM_BUFFER, projectionBindingIndex, projectionUBO);

	glGenBuffers(1, &modelViewUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, modelViewUBO);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), NULL, GL_STREAM_DRAW);
	modelViewBindingIndex = 1;
	glBindBufferBase(GL_UNIFORM_BUFFER, modelViewBindingIndex, modelViewUBO);

	//init lighting

	int numActiveLights = 1;
	
	glGenBuffers(1, &numLightsUBO);
    glBindBuffer(GL_UNIFORM_BUFFER, numLightsUBO);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(int), &numActiveLights, GL_STATIC_DRAW);
	numLightsBindingIndex = 3;
    glBindBufferBase(GL_UNIFORM_BUFFER, numLightsBindingIndex, numLightsUBO);

	GL_Lighting lightingGL;
	GL_Light lightGL;

	lightGL.lightIntensity = glm::vec4(.5, .5, .5, 1);
	float halfDistance = 100.0f;
	lightGL.lightAttenuation = 1.0f / (halfDistance * halfDistance);
	lightGL.cameraSpaceLightPos = glm::vec4(0,0,0,1);
	lightingGL.lights[0] = lightGL;

	glGenBuffers(1, &lightingUBO);
    glBindBuffer(GL_UNIFORM_BUFFER, lightingUBO);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(GL_Lighting), &lightingGL, GL_STATIC_DRAW);
	lightingBindingIndex = 4;
    glBindBufferBase(GL_UNIFORM_BUFFER, lightingBindingIndex, lightingUBO);

	//init material

	MaterialBlock material;
	material.diffuseColor = glm::vec4(1,0,0,1);
	material.specularColor = glm::vec4(1,1,1,1);
	material.specularShininess = 0.1f;

	glGenBuffers(1, &materialUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, materialUBO);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(MaterialBlock), &material, GL_STATIC_DRAW);
	materialBindingIndex = 5;
	glBindBufferBase(GL_UNIFORM_BUFFER, materialBindingIndex, materialUBO);

	//init other things
	Globals::shaderState.initialize();
	Globals::meshLibrary.initialize();
	camera.activate();


}

void resize(int w, int h)
{
	projectionMatrix[0].x = frustumScale * (h / (float)w);
    projectionMatrix[1].y = frustumScale;

	width = w;
	height = h;

    glViewport(0, 0, (GLsizei) w, (GLsizei) h);


	//set projection matrix through orphaning
	glBindBuffer(GL_UNIFORM_BUFFER, projectionUBO);
	glm::mat4* Pointer = (glm::mat4*)glMapBufferRange(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), 
			GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT | GL_MAP_UNSYNCHRONIZED_BIT);

		*Pointer = projectionMatrix;

	glUnmapBuffer(GL_UNIFORM_BUFFER);


}

void enterFrame()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClearDepth(1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//update the model-view matrix through orphaning
	glBindBuffer(GL_UNIFORM_BUFFER, modelViewUBO);
	glm::mat4* Pointer = (glm::mat4*)glMapBufferRange(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), 
			GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT | GL_MAP_UNSYNCHRONIZED_BIT);

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = Globals::viewMatrix;
		glm::mat4 modelView = view * model;

		*Pointer = modelView;

	glUnmapBuffer(GL_UNIFORM_BUFFER);

	Globals::shaderState.prepareForRender();
	Globals::meshLibrary.render();

}


/*-----------------------------
		Input/Output 
-----------------------------*/

int main (int argc, char **argv)
{
    // Create the main rendering window
	int width = 800;
	int height = 800;
	sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode(width, height, 32), "Instanced Culling", 6UL, sf::WindowSettings(24U, 8U, 4U));
	
    window->SetActive();
    //window->UseVerticalSync(true);
    //window->SetFramerateLimit(100);

	init();
	resize(width, height);

	bool mouseDown = false;
	int prevMouseX = 0;
	int prevMouseY = 0;

    while (window->IsOpened())
    {

		enterFrame();
        window->Display();

        sf::Event myEvent;
        while (window->GetEvent(myEvent))
        {
			switch(myEvent.Type)
			{
				case sf::Event::Resized:

					resize(window->GetWidth(), window->GetHeight());
					break;

				case sf::Event::MouseButtonPressed:

					if(myEvent.MouseButton.Button == sf::Mouse::Left)
					{
						mouseDown = true;
						prevMouseX = myEvent.MouseButton.X;
						prevMouseY = myEvent.MouseButton.Y;
					}
					break;

				case sf::Event::MouseButtonReleased:

					if(myEvent.MouseButton.Button == sf::Mouse::Left)
					{
						mouseDown = false;
					}
					break;

				case sf::Event::MouseMoved:

					if(mouseDown)
					{
						int x = myEvent.MouseMove.X;
						int y = myEvent.MouseMove.Y;

						int mouseXDiff = (x - prevMouseX);
						int mouseYDiff = (y - prevMouseY);

						float scaleFactor = .008f;
						float mouseXDifference = -mouseXDiff * scaleFactor;
						float mouseYDifference = -mouseYDiff * scaleFactor;
						camera.rotate(mouseXDifference,mouseYDifference);

						prevMouseX = x;
						prevMouseY = y;
					}
					break;

				case sf::Event::KeyPressed:

					if(myEvent.Key.Code == sf::Key::Space)
					{
						std::cout << "pressed spacebar" << std::endl;
					}
					break;

				case sf::Event::Closed:

					window->Close();
					break;
			}
        }
    }
}
