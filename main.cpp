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
#include "src/imgui.h"
#include "src/imgui_impl_glfw.h"
#include "src/imgui_impl_opengl3.h"

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
SpaceNode* selectedNode = nullptr;

bool show_demo_window = true;
bool show_another_window = false;
ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

glm::vec3 cameraPosition;

float cameraDistance = 10;

void generateBackGroundImage();

std::list<SpaceNode*> space_nodes;


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

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();


    return 0;
}


glm::uint textureID;




void init()
{
    if (!glfwInit())
        throw "Could not initialize glwf";
    window = glfwCreateWindow(1920, 1080, "SpaceSim", NULL, NULL);
    glfwMakeContextCurrent(window);
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
            if (key == GLFW_KEY_LEFT_SHIFT)
            {
                if (action == GLFW_PRESS)
                {
                    spaceShip::on_press_shift();
                }
                else if (action == GLFW_RELEASE)
                {
                    spaceShip::on_release_shift();
                }
            }
            if (key == GLFW_KEY_LEFT_CONTROL)
            {
                if (action == GLFW_PRESS)
                {
                    spaceShip::on_press_control();
                }
                else if (action == GLFW_RELEASE)
                {
                    spaceShip::on_release_control();
                }
            }
        });
    

    //baseNode = new ManSatellite("models/ship/shipA_OBJ.obj", nullptr, 0, 0,10);

    std::string sunTexture = "resources/8k_sun.png";
    baseNode = new Planetoid("sun",&sunTexture,0, glm::vec3(10,10,10), true);

   // baseNode = new ManSatellite("models/monkey/monkey.obj", 10, glm::vec3(1, 1, 1));
	
    std::string mercureyTexture = "resources/8k_mercury.png";
    Planetoid* mercury = new Planetoid("mercury", &mercureyTexture,24, glm::vec3(1, 1, 1), false);

    std::string venusTexture = "resources/8k_venus_surface.png";
    Planetoid* venus = new Planetoid("venus",&venusTexture, 48, glm::vec3(1, 1, 1), false);

    std::string marsTexture = "resources/2k_mars.png";
    Planetoid* mars = new Planetoid("mars",&marsTexture, 48, glm::vec3(1, 1, 1), false);

    std::string earthTexture = "resources/2k_earth_daymap.png";
    Planetoid* earth = new Planetoid("earth",&earthTexture, 48, glm::vec3(1, 1, 1), false);

    std::string moonTexture = "resources/2k_moon.png";
    Planetoid* moon = new Planetoid("moon",&moonTexture,  48, glm::vec3(0.3f, 0.3f, 0.3f), false);
	 
    ManSatellite* mSatellite = new ManSatellite("mSatellite","models/sat/sat.obj", 120, glm::vec3(0.5f, 0.5f, 0.5f));
	
	
    std::string jupiterTexture = "resources/2k_jupiter.png";
    Planetoid* jupiter = new Planetoid("jupiter",&jupiterTexture, 48, glm::vec3(3, 3, 3), false);

    std::string jmoonTexture = "resources/2k_moon.png";
    Planetoid* jmoon = new Planetoid("jmoon", &jmoonTexture, 48, glm::vec3(0.3f, 0.3f, 0.3f), false);

	

    ManSatellite* jSattelite = new ManSatellite("jSattelite","models/sat/sat.obj", 120, glm::vec3(1, 1, 1));

    std::string saturnTexture = "resources/2k_saturn.png";
    Planetoid* saturn = new Planetoid("saturn",&saturnTexture, 48, glm::vec3(2, 2, 2), false);

    std::string uranusTexture = "resources/2k_uranus.png";
    Planetoid* uranus = new Planetoid("uranus",&uranusTexture, 48, glm::vec3(2, 2, 2), false);
	
	
    std::string neptuneTexture = "resources/2k_neptune.png";
    Planetoid* neptune = new Planetoid("neptune",&neptuneTexture, 48, glm::vec3(2, 2, 2), false);


    baseNode->add_sat(mercury,40, 97);
    baseNode->add_sat(venus, 60, 73);
    baseNode->add_sat(mars, 80, 53);
    baseNode->add_sat(earth, 100, 29);

    earth->add_sat(moon, 7, 89 );
    moon->add_sat(mSatellite, 1, 120);
	
    baseNode->add_sat(jupiter, 140, 23);
    jupiter->add_sat(jSattelite,5,120);
    jupiter->add_sat(jmoon, 10, 200);
	
    baseNode->add_sat(saturn, 160, 11);
    baseNode->add_sat(uranus, 180, 7);
    baseNode->add_sat(neptune, 200, 3);

    space_nodes.push_back(baseNode);
    space_nodes.push_back(mercury);
    space_nodes.push_back(venus);
    space_nodes.push_back(mars);
    space_nodes.push_back(earth);
    space_nodes.push_back(moon);
    space_nodes.push_back(mSatellite);
    space_nodes.push_back(jupiter);
    space_nodes.push_back(jSattelite);
    space_nodes.push_back(jmoon);
    space_nodes.push_back(saturn);
    space_nodes.push_back(uranus);
    space_nodes.push_back(neptune);
    
	

	
    int viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    projectionMatrix = glm::perspective(glm::radians(fov), viewport[2] / (float)viewport[3], 0.01f, 500.0f);
    viewMatrix = glm::lookAt(CAMERA_POS, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

    rotation = 0;


    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char* glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui::StyleColorsDark();
	
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
    
	
}


void update(float timeMillis)
{
    glfwPollEvents();
    baseNode->update(timeMillis);
    spaceShip::update_ship(timeMillis);

   


    if (!selectedNode)
    {
        glm::vec3 rotationAngles = spaceShip::getShipRotation();

        float x = sin(glm::radians(rotationAngles[1])) * (cameraDistance);
        float z = cos(glm::radians(rotationAngles[1])) * (cameraDistance);
        glm::vec3 offset(x, 5, z);
        offset += spaceShip::getShipPosition();
        cameraPosition = offset;
        viewMatrix = glm::lookAt(cameraPosition, spaceShip::getShipPosition(), glm::vec3(0, 1, 0));
    }
    else
    {
        glm::vec3 rotationAngles = selectedNode->get_rotation();

        float x = sin((rotationAngles[1])) * (cameraDistance);
        float z = cos((rotationAngles[1])) * (cameraDistance);
        glm::vec3 offset(x, 5, z);
        offset += selectedNode->get_position();
        cameraPosition = offset;
    	
        viewMatrix = glm::lookAt(cameraPosition, selectedNode->get_position(), glm::vec3(0, 1, 0));
    }
	
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

     float angle;
	
    if (!selectedNode)
    {
         angle = spaceShip::getShipRotation()[1];
    }else
    {
        angle = glm::degrees( selectedNode->get_rotation()[1]);
    }
    
	
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

    glfwPollEvents();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    static float f = 0.0f;
    static int counter = 0;

	
    ImGui::Begin("select a planet to view"); 
    if (ImGui::Button("spaceShip"))
    {
        selectedNode = nullptr;
    }                            // Buttons ret

    for (SpaceNode* space_node : space_nodes)
    {
        if (ImGui::Button(space_node->get_name().c_str()))
        {
            selectedNode = space_node;
        }                            // Buttons return true when clicked (most widgets return true when edited/activated)
    }
    
    ImGui::End();

    ImGui::Render();
    
  
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	
}

