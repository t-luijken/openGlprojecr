#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "tigl.h"
#include "FpsCam.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
using tigl::Vertex;

#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "opengl32.lib")

GLFWwindow* window;

void init();
void update();
void draw();
void draw_cube(float x, float y, float z, int id);

int world[20][20][10];

void generateworld()
{
	for (int i = 0; i < 20; ++i)
	{
		for (int j = 0; j < 20; ++j)
		{
			for (int k = 0; k < 10; ++k)
			{
				if (k < 5)
				{
                    world[i][j][k] = 2;
				}else if(k == 5)
				{
                    world[i][j][k] = 1;
				}else
				{
                    world[i][j][k] = -1;
				}
			}
		}
	}

	
}


int main(void)
{
    generateworld();
    if (!glfwInit())
        throw "Could not initialize glwf";
    window = glfwCreateWindow(1400, 800, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        throw "Could not initialize glwf";
    }
    glfwMakeContextCurrent(window);

    tigl::init();

    init();

	while (!glfwWindowShouldClose(window))
	{
		update();
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
    glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        if (key == GLFW_KEY_ESCAPE)
            glfwSetWindowShouldClose(window, true);
    });
    camera = new FpsCam(window);

    glm::uint textureID;
    glGenTextures(1, &textureID);

    glBindTexture(GL_TEXTURE_2D, textureID);

    int width, height, bpp;
    char* imgData = (char*) stbi_load("resources/tiles.png",&width, &height, &bpp,4);
    glTexImage2D( GL_TEXTURE_2D,
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
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}


void update()
{
    camera->update(window);

}

void draw()
{
   
    glClearColor(0.3f, 0.4f, 0.6f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   
	
    glEnable(GL_TEXTURE_2D);
    int viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    glm::mat4 projection = glm::perspective(glm::radians(75.0f), viewport[2] / (float)viewport[3], 0.01f, 100.0f);

    tigl::shader->setProjectionMatrix(projection);
    tigl::shader->setViewMatrix(camera->getMatrix());
    tigl::shader->setModelMatrix(glm::mat4(1.0f));

    //tigl::shader->enableColor(true);
    tigl::shader->enableTexture(true);
    glEnable(GL_DEPTH_TEST);
	

   
    for (int i = 0; i < 20; ++i)
    {
        for (int j = 0; j < 20; ++j)
        {
            for (int k = 0; k < 10; ++k)
            {
                int tile = world[i][j][k];
                if (tile != -1)
                {
                    draw_cube(i, k, j, tile);
                }
            	
            }
        }
    }
	

    
}

void draw_cube(float x, float y, float z, int id)
{
    float x00, x01, x10, x11, y00, y01, y10, y11;

    float xLeft, xRight, yTop, yBottom;
	
    xLeft = 0.0625f * (id % 16);
    
    yTop = id / 16;
    
    xRight = xLeft + 0.0625f;
    
    yBottom = yTop + 0.0625f;
   


	
    tigl::begin(GL_QUADS);

    tigl::addVertex(Vertex::PT(glm::vec3(x + 0, y + 0, z + 0), glm::vec2(xLeft, yTop    )));
    tigl::addVertex(Vertex::PT(glm::vec3(x + 0, y + 1, z + 0), glm::vec2(xLeft, yBottom )));
    tigl::addVertex(Vertex::PT(glm::vec3(x + 1, y + 1, z + 0), glm::vec2(xRight, yBottom)));
    tigl::addVertex(Vertex::PT(glm::vec3(x + 1, y + 0, z + 0), glm::vec2(xRight, yTop   )));

    tigl::addVertex(Vertex::PT(glm::vec3(x + 0, y + 0, z + 1), glm::vec2(xLeft, yTop)));
    tigl::addVertex(Vertex::PT(glm::vec3(x + 0, y + 1, z + 1), glm::vec2(xLeft, yBottom)));
    tigl::addVertex(Vertex::PT(glm::vec3(x + 1, y + 1, z + 1), glm::vec2(xRight, yBottom)));
    tigl::addVertex(Vertex::PT(glm::vec3(x + 1, y + 0, z + 1), glm::vec2(xRight, yTop)));

    tigl::addVertex(Vertex::PT(glm::vec3(x + 0, y + 0, z + 0), glm::vec2(xLeft, yTop)));
    tigl::addVertex(Vertex::PT(glm::vec3(x + 0, y + 0, z + 1), glm::vec2(xLeft, yBottom)));
    tigl::addVertex(Vertex::PT(glm::vec3(x + 1, y + 0, z + 1), glm::vec2(xRight, yBottom)));
    tigl::addVertex(Vertex::PT(glm::vec3(x + 1, y + 0, z + 0), glm::vec2(xRight, yTop)));

    tigl::addVertex(Vertex::PT(glm::vec3(x + 1, y + 1, z + 0), glm::vec2(xLeft, yTop)));
    tigl::addVertex(Vertex::PT(glm::vec3(x + 1, y + 1, z + 1), glm::vec2(xLeft, yBottom)));
    tigl::addVertex(Vertex::PT(glm::vec3(x + 0, y + 1, z + 1), glm::vec2(xRight, yBottom)));
    tigl::addVertex(Vertex::PT(glm::vec3(x + 0, y + 1, z + 0), glm::vec2(xRight, yTop)));

    tigl::addVertex(Vertex::PT(glm::vec3(x + 1, y + 1, z + 0), glm::vec2(xLeft, yTop)));
    tigl::addVertex(Vertex::PT(glm::vec3(x + 1, y + 1, z + 1), glm::vec2(xLeft, yBottom)));
    tigl::addVertex(Vertex::PT(glm::vec3(x + 1, y + 0, z + 1), glm::vec2(xRight, yBottom)));
    tigl::addVertex(Vertex::PT(glm::vec3(x + 1, y + 0, z + 0), glm::vec2(xRight, yTop)));

    tigl::addVertex(Vertex::PT(glm::vec3(x + 0, y + 1, z + 0), glm::vec2(xLeft, yTop)));
    tigl::addVertex(Vertex::PT(glm::vec3(x + 0, y + 1, z + 1), glm::vec2(xLeft, yBottom)));
    tigl::addVertex(Vertex::PT(glm::vec3(x + 0, y + 0, z + 1), glm::vec2(xRight, yBottom)));
    tigl::addVertex(Vertex::PT(glm::vec3(x + 0, y + 0, z + 0), glm::vec2(xRight, yTop)));
    tigl::end();

}