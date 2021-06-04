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
#include "SpaceShip.h"
#include "stb_image.h"

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
float fov = 75.0f;

glm::mat4 viewMatrix;
glm::mat4 projectionMatrix;
static float rotation = 0;

SpaceNode* baseNode;
ObjModel* obj_model;


glm::vec3 cameraPosition;

float cameraDistance = 10;

void generateBackGroundImage();


int main(void)
{

    init();

	while (!glfwWindowShouldClose(window))
	{
        double currentTime = glfwGetTime();
        double deltaTime = currentTime - lastTime;
        lastTime = currentTime;

      //  std::cout << "fps:";
       // std::cout << 1.0f/deltaTime<<std::endl;
		
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

    spaceShip::init_ship();

    generateBackGroundImage();

    glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            if (key == GLFW_KEY_ESCAPE)
                glfwSetWindowShouldClose(window, true);
            if (key == GLFW_KEY_A)
            {
	            if (action == GLFW_PRESS)
	            {
                    spaceShip::ship_left();
	            }else if (action == GLFW_RELEASE)
	            {
                    spaceShip::ship_left_release();
	            }
               
            }
            if (key == GLFW_KEY_D)
            {
                if (action == GLFW_PRESS)
                {
                    spaceShip::ship_right();
                }
                else if (action == GLFW_RELEASE)
                {
                    spaceShip::ship_right_release();
                }
            }
            if (key == GLFW_KEY_W)
            {
                if (action == GLFW_PRESS)
                {
                    spaceShip::ship_forwards();
                }
                else if (action == GLFW_RELEASE)
                {
                    spaceShip::ship_forwards_release();
                }
            }
            if (key == GLFW_KEY_S)
            {
                if (action == GLFW_PRESS)
                {
                    spaceShip::ship_backwards();
                }
                else if (action == GLFW_RELEASE)
                {
                    spaceShip::ship_backwards_release();
                }
            }
        });
    

    //baseNode = new ManSatellite("models/ship/shipA_OBJ.obj", nullptr, 0, 0,10);

    std::string sunTexture = "resources/2k_sun.png";
    baseNode = new Planetoid(&sunTexture,0, glm::vec3(10,10,10), true);

    std::string mercureyTexture = "resources/8k_mercury.png";
    Planetoid* mercury = new Planetoid(&mercureyTexture,24, glm::vec3(1, 1, 1), false);

    std::string venusTexture = "resources/8k_venus_surface.png";
    Planetoid* venus = new Planetoid(&venusTexture, 48, glm::vec3(1, 1, 1), false);

    std::string marsTexture = "resources/2k_mars.png";
    Planetoid* mars = new Planetoid(&marsTexture, 48, glm::vec3(1, 1, 1), false);

    std::string earthTexture = "resources/2k_earth_daymap.png";
    Planetoid* earth = new Planetoid(&earthTexture, 48, glm::vec3(1, 1, 1), false);

    std::string moonTexture = "resources/2k_moon.png";
    Planetoid* moon = new Planetoid(&moonTexture,  48, glm::vec3(0.3f, 0.3f, 0.3f), false);

	
    std::string jupiterTexture = "resources/2k_jupiter.png";
    Planetoid* jupiter = new Planetoid(&jupiterTexture, 48, glm::vec3(3, 3, 3), false);

    std::string saturnTexture = "resources/2k_saturn.png";
    Planetoid* saturn = new Planetoid(&saturnTexture, 48, glm::vec3(2, 2, 2), false);

    std::string uranusTexture = "resources/2k_uranus.png";
    Planetoid* uranus = new Planetoid(&uranusTexture, 48, glm::vec3(2, 2, 2), false);
	
	
    std::string neptuneTexture = "resources/2k_neptune.png";
    Planetoid* neptune = new Planetoid(&neptuneTexture, 48, glm::vec3(2, 2, 2), false);


    baseNode->add_sat(mercury,20, 100);
    baseNode->add_sat(venus, 30, 70);
    baseNode->add_sat(mars, 40, 50);
    baseNode->add_sat(earth, 50, 30);

    earth->add_sat(moon, 5, 100 );
	
    baseNode->add_sat(jupiter, 70, 20);
    baseNode->add_sat(saturn, 80, 10);
    baseNode->add_sat(uranus, 90, 7);
    baseNode->add_sat(neptune, 100, 3);


	

    camera = new FpsCam(window);
	
    int viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    projectionMatrix = glm::perspective(glm::radians(fov), viewport[2] / (float)viewport[3], 0.01f, 500.0f);
    viewMatrix = glm::lookAt(CAMERA_POS, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

    rotation = 0;
}


void update(float timeMillis)
{
    camera->update(window);
    baseNode->update(timeMillis);
    spaceShip::update_ship(timeMillis);

    glm::vec3 rotationAngles = spaceShip::getShipRotation();
	
    float x = sin(glm::radians(rotationAngles[1])) * (cameraDistance);
    float z = cos(glm::radians(rotationAngles[1])) * (cameraDistance);
    glm::vec3 offset(x, 5, z);
    offset += spaceShip::getShipPosition();

    cameraPosition = offset;
	
}
GLuint textureId;

void generateBackGroundImage()
{
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);


    int width, height, bpp;


    char* imgData = (char*)stbi_load("resources/8k_stars_milky_way.png", &width, &height, &bpp, 4);

    glTexImage2D(GL_TEXTURE_2D,
        0,
        GL_RGBA,
        width,
        height,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        imgData
    );
    assert(imgData);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_image_free(imgData);


	
}

void draw()
{
    glClearColor(0.3f, 0.4f, 0.6f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
    glClear(GL_DEPTH_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);

    tigl::shader->enableTexture(true);
	
    tigl::shader->setProjectionMatrix(glm::ortho(-1, 1, -1, 1, 300, -3000));
     tigl::shader->setViewMatrix(glm::lookAt(glm::vec3(0,0,7),glm::vec3(0,0,0),glm::vec3(0,1,0)));
     glBindTexture(GL_TEXTURE_2D, textureId);


     float angle = spaceShip::getShipRotation()[1];
	
     float leftBound = angle - (fov / 2);
     float righBound = angle + (fov / 2);

     float leftBoundN = leftBound / 360.0f;
     float rightBoundN = righBound / 360.0f;
	
    tigl::begin(GL_QUADS);
    tigl::addVertex(Vertex::PT(glm::vec3(1, 2, -2000), glm::vec2(leftBoundN,0)));

    tigl::addVertex(Vertex::PT(glm::vec3(1, -2, -2000), glm::vec2(leftBoundN, 1)));

    tigl::addVertex(Vertex::PT(glm::vec3(-1, -2, -2000), glm::vec2(rightBoundN, 1)));
	
    tigl::addVertex(Vertex::PT(glm::vec3(-1, 2, -2000), glm::vec2(rightBoundN, 0)));
    tigl::end();
	
    glBindTexture(GL_TEXTURE_2D, 0);

    tigl::shader->enableTexture(false);

    tigl::shader->setProjectionMatrix(projectionMatrix);
    tigl::shader->setViewMatrix(viewMatrix);
	
	

    viewMatrix = glm::lookAt(cameraPosition, spaceShip::getShipPosition(), glm::vec3(0, 1, 0));

    //glEnable(GL_NORMALIZE);
	
    tigl::shader->setProjectionMatrix(projectionMatrix);
    tigl::shader->setViewMatrix(viewMatrix);

    tigl::shader->enableLighting(true);
    tigl::shader->setLightCount(1);

    tigl::shader->setLightDirectional(0, false);
    tigl::shader->setLightPosition(0, glm::vec3(0, 0, 0));
    tigl::shader->setLightAmbient(0, glm::vec3(0.1f, 0.1f, 0.15f));
    tigl::shader->setLightDiffuse(0, glm::vec3(0.8f, 0.8f, 0.8f));
    tigl::shader->setLightSpecular(0, glm::vec3(0, 0, 0));
    tigl::shader->setShinyness(32.0f);
	
    glShadeModel(GL_SMOOTH);

    glm::mat4 modelMatrix = glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0, 1, 0));

    tigl::shader->setModelMatrix(modelMatrix);

    tigl::shader->enableColor(true);

  
	
    glEnable(GL_DEPTH_TEST);

    baseNode->draw();
    spaceShip::draw_ship();
 

}

