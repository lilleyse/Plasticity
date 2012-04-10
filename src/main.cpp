#include <gl3w/gl3w.h>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <glm/glm.hpp>
#include "Utils.h"
#include "Globals.h"
#include "Cameras/Camera3rdPerson.h"
#include "Physics/PhysicsWorld.h"
#include "Physics/PhysicsObject.h"


namespace
{
	glm::mat4 projectionMatrix;
	float fzNear;
    float fzFar;
    float frustumScale;
	int width;
	int height;

	GLuint projectionUBO;
	GLuint projectionBindingIndex = 0;
	
	struct ModelViewStruct
	{
		glm::mat4 normalMatrix;
		glm::mat4 matrix;
		
	};

	GLuint modelViewUBO;
	GLuint modelViewBindingIndex = 1;

	GLuint numLightsUBO;
	GLuint numLightsBindingIndex = 3;
	
	GLuint lightingUBO;
	GLuint lightingBindingIndex = 4;

	GLuint materialUBO;
	GLuint materialBindingIndex = 5;

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

	//Camera
	Camera3rdPerson camera;

	//Physics
	PhysicsWorld* physicsWorld;
}

void initGL()
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
    glBindBufferBase(GL_UNIFORM_BUFFER, projectionBindingIndex, projectionUBO);

	glGenBuffers(1, &modelViewUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, modelViewUBO);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(ModelViewStruct), NULL, GL_STREAM_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, modelViewBindingIndex, modelViewUBO);

	//init lighting

	int numActiveLights = 1;
	
	glGenBuffers(1, &numLightsUBO);
    glBindBuffer(GL_UNIFORM_BUFFER, numLightsUBO);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(int), &numActiveLights, GL_STATIC_DRAW);
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
    glBindBufferBase(GL_UNIFORM_BUFFER, lightingBindingIndex, lightingUBO);

	//init material

	MaterialBlock material;
	material.diffuseColor = glm::vec4(1,0,0,1);
	material.specularColor = glm::vec4(1,1,1,1);
	material.specularShininess = 0.1f;

	glGenBuffers(1, &materialUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, materialUBO);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(MaterialBlock), &material, GL_STATIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, materialBindingIndex, materialUBO);

	//init other things
	Globals::shaderState.initialize();
	Globals::meshLibrary.initialize();
	camera.activate();
}
void initPhysics()
{
	physicsWorld = new PhysicsWorld();

	//ground plane
	PhysicsObject* floor = new PhysicsObject(PRIMITIVE_BOX,0.0f,0.1f,0.8f);
	floor->setScale(glm::vec3(10.0f,1.0f,10.0f));
	floor->attachMesh(Globals::meshLibrary.getMesh(1));
	physicsWorld->addObject(floor);

	//balls
	for(int i = 0; i < 50; i++)
	{
		PhysicsObject* object;

		int objectType = i % 2;
		if(objectType == 0) //sphere
		{
			object = new PhysicsObject(PRIMITIVE_SPHERE, 1.0f,0.1f,0.8f);
			object->attachMesh(Globals::meshLibrary.getMesh(3));
		}
		else if(objectType == 1) //cube
		{
			object = new PhysicsObject(PRIMITIVE_BOX, 1.0f,0.1f,0.8f);
			object->attachMesh(Globals::meshLibrary.getMesh(1));
		}
		 
		object->setTranslationY((float)(i*2) + 3.0f);
		physicsWorld->addObject(object);
	}
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

	Globals::shaderState.prepareForRender();
	
	physicsWorld->update();
	std::vector<PhysicsObject*>& objects = physicsWorld->getObjects();
	for(unsigned int i = 0; i < objects.size(); i++)
	{
		//get model view matrix
		PhysicsObject* object = objects.at(i);
		object->updateTransformationMatrix();
		glm::mat4 model = object->getTransformationMatrix();
		glm::mat4 view = Globals::viewMatrix;
		glm::mat4 modelView = view * model;
		ModelViewStruct modelViewStruct;
		modelViewStruct.matrix = modelView;
		modelViewStruct.normalMatrix = glm::mat4(glm::transpose(glm::inverse(glm::mat3(modelView))));

		glBindBuffer(GL_UNIFORM_BUFFER, modelViewUBO);
		ModelViewStruct* modelViewPointer = (ModelViewStruct*)glMapBufferRange(GL_UNIFORM_BUFFER, 0, sizeof(ModelViewStruct), 
				GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT | GL_MAP_UNSYNCHRONIZED_BIT);
		
		*modelViewPointer = modelViewStruct;
		glUnmapBuffer(GL_UNIFORM_BUFFER);

		object->getAttachedMesh()->render();
	}
}


/*-----------------------------
		Input/Output 
-----------------------------*/

int main (int argc, char **argv)
{
    // Create the main rendering window
	int width = 800;
	int height = 800;
	sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode(width, height, 32), "Plasticity", 6UL, sf::WindowSettings(24U, 8U, 4U));
	
    window->SetActive();
    window->UseVerticalSync(true);
    window->SetFramerateLimit(60);

	initGL();
	initPhysics();

	resize(width, height);

	bool mouseDown = false;
	int prevMouseX = 0;
	int prevMouseY = 0;

    int numFrames = 0;
	sf::Clock clock;

    while (window->IsOpened())
    {

		//framerate
		numFrames++;
		if(clock.GetElapsedTime() > 1.0f)
		{
			std::cout << "fps: " << numFrames << std::endl;
			numFrames = 0;
			clock.Reset();
		}

		enterFrame();
        window->Display();

		
		if(window->GetInput().IsKeyDown(sf::Key::W))
		{
			camera.zoom(2);
		}
		else if(window->GetInput().IsKeyDown(sf::Key::S))
		{
			camera.zoom(-1);
		}
		else if(window->GetInput().IsKeyDown(sf::Key::A))
		{
			camera.pan(-1, 0);
		}
		else if(window->GetInput().IsKeyDown(sf::Key::D))
		{
			camera.pan(1, 0);
		}


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

				case sf::Event::MouseWheelMoved:
					{
						int delta = myEvent.MouseWheel.Delta;
						float scaleFactor = 1.0f;
						camera.zoom(scaleFactor*delta);
					}
					break;
				case sf::Event::KeyPressed:

					break;

				case sf::Event::Closed:

					window->Close();
					break;
			}
        }
    }
}

