#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "tigl.h"
#include "FpsCam.h"
#include <iostream>
#include <map>
#include <glm/gtc/matrix_transform.hpp>
#include "SpaceNode.hpp"
#include "ManSatellite.hpp"
#include "Planetoid.h"

using tigl::Vertex;

#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "opengl32.lib")

GLFWwindow* window;
#define CAMERA_POS glm::vec3(0.0f,10,10)

void init();
void update(float timeMillis);
void draw();
void draw_cube(float x, float y, float z, int id);
static double lastTime;

glm::mat4 viewMatrix;
glm::mat4 projectionMatrix;
static float rotation = 0;

SpaceNode* baseNode;
ObjModel* obj_model;




int main(void)
{

    init();

	while (!glfwWindowShouldClose(window))
	{
        double currentTime = glfwGetTime();
        double deltaTime = currentTime - lastTime;
        lastTime = currentTime;
		update(deltaTime);
		draw();
		glfwSwapBuffers(window);
		glfwPollEvents();
		
	}

	glfwTerminate();


    return 0;
}

FpsCam* camera;
glm::uint textureID;





void init()
{
    if (!glfwInit())
        throw "Could not initialize glwf";
    window = glfwCreateWindow(1920, 1080, "SpaceSim", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        throw "Could not initialize glwf";
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    std::cout << glGetString(GL_VERSION) << std::endl;

    tigl::init();

    glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            if (key == GLFW_KEY_ESCAPE)
                glfwSetWindowShouldClose(window, true);
        });
    

    //baseNode = new ManSatellite("models/ship/shipA_OBJ.obj", nullptr, 0, 0,10);

    baseNode = new Planetoid("resources/2k_sun.jpg",  0,0,0, glm::vec3(10,10,10));
    ManSatellite* sat = new ManSatellite("models/ship/shipA_OBJ.obj", 30, 10,10, glm::vec3(0.01f, 0.01f,0.01f));

    Planetoid* moon = new Planetoid("test", 10, 100, 300,glm::vec3(1,1,1));

    ManSatellite* sat2 = new ManSatellite("models/ship/shipA_OBJ.obj", 15, 10, 10, glm::vec3(0.01f, 0.01f, 0.01f));


    sat->add_sat(moon);
    moon->add_sat(sat2);
	
    baseNode->add_sat(sat);


	

    camera = new FpsCam(window);
	
    int viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    projectionMatrix = glm::perspective(glm::radians(75.0f), viewport[2] / (float)viewport[3], 0.01f, 500.0f);
    viewMatrix = glm::lookAt(CAMERA_POS, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

    rotation = 0;
}


void update(float timeMillis)
{
    camera->update(window);
    baseNode->update(timeMillis);

}

void draw()
{
    glClearColor(0.3f, 0.4f, 0.6f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    tigl::shader->setProjectionMatrix(projectionMatrix);
    tigl::shader->setViewMatrix(camera->getMatrix());


    glm::mat4 modelMatrix = glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0, 1, 0));
    //modelMatrix = glm::translate(modelMatrix, glm::vec3(-10, -10, -10));
    tigl::shader->setModelMatrix(modelMatrix);

    tigl::shader->enableColor(true);

    glEnable(GL_DEPTH_TEST);

    baseNode->draw();
 
 

}

