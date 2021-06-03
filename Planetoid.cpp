#include "Planetoid.h"

#include "stb_image.h"
#include "tigl.h"

#define SCALE_FACTOR 5

const float PI = (2 * acos(0.0));

std::vector<tigl::Vertex> vertices;

tigl::VBO* vbo;


glm::vec3 getXYZfromAngles(float phi, float theta)
{
    float x = cos(phi) * cos(theta);
    float y = cos(phi) * sin(theta);
    float z = sin(phi);
    return glm::vec3(x, z, y);

}
GLuint texId;

Planetoid::Planetoid(const char* textureLink,  float distance, float speed, float rotation_speed, glm::vec3 scale) : SpaceNode(speed, distance, rotation_speed, scale)
{

    glGenTextures(1, &texId);
    glBindTexture(GL_TEXTURE_2D, texId);
	
	
    int width, height, bpp;
	
	
    char* imgData =(char*) stbi_load(textureLink, &width, &height, &bpp, 4);
	
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
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_image_free(imgData);

	
    for (int i = 0; i < 360; i += SCALE_FACTOR)
    {
        for (int j = 0; j < 180; j += SCALE_FACTOR)
        {

          //  float Rand_red = ((float)(rand() % 255)) / 255.0f;

          //  float Rand_green = ((float)(rand() % 255)) / 255.0f;

         //   float Rand_blue = ((float)(rand() % 255)) / 255.0f;


           // vertices.push_back(tigl::Vertex::PC(getXYZfromAngles(glm::radians((float)i), glm::radians((float)j)),                                   glm::vec4(Rand_red, Rand_blue, Rand_green, 1)));

           // vertices.push_back(tigl::Vertex::PC(getXYZfromAngles(glm::radians((float)(i + SCALE_FACTOR)), glm::radians((float)j)),                  glm::vec4(Rand_red, Rand_blue, Rand_green, 1)));

           // vertices.push_back(tigl::Vertex::PC(getXYZfromAngles(glm::radians((float)(i + SCALE_FACTOR)), glm::radians((float)(j + SCALE_FACTOR))), glm::vec4(Rand_red, Rand_blue, Rand_green, 1)));

          //  vertices.push_back(tigl::Vertex::PC(getXYZfromAngles(glm::radians((float)i), glm::radians((float)(j + SCALE_FACTOR))),                  glm::vec4(Rand_red, Rand_blue, Rand_green, 1)));


        /*	float texture_step_height = 1 / 360;
           float texture_step_width = 1/180;
           
           vertices.push_back(tigl::Vertex::PT(getXYZfromAngles(glm::radians((float)i), glm::radians((float)j)), glm::vec2(i * texture_step_height, j * texture_step_width)));

           vertices.push_back(tigl::Vertex::PT(getXYZfromAngles(glm::radians((float)(i + SCALE_FACTOR)), glm::radians((float)j)), glm::vec2((i+1) * texture_step_height, j * texture_step_width)));

           vertices.push_back(tigl::Vertex::PT(getXYZfromAngles(glm::radians((float)(i + SCALE_FACTOR)), glm::radians((float)(j + SCALE_FACTOR))), glm::vec2((i + 1) * texture_step_height, (j+1) * texture_step_width)));

           vertices.push_back(tigl::Vertex::PT(getXYZfromAngles(glm::radians((float)i), glm::radians((float)(j + SCALE_FACTOR))), glm::vec2(i * texture_step_height, (j + 1) * texture_step_width)));*/



           vertices.push_back(tigl::Vertex::PT(getXYZfromAngles(glm::radians((float)i), glm::radians((float)j)), glm::vec2(0,0)));

           vertices.push_back(tigl::Vertex::PT(getXYZfromAngles(glm::radians((float)(i + SCALE_FACTOR)), glm::radians((float)j)), glm::vec2(1, 0)));

           vertices.push_back(tigl::Vertex::PT(getXYZfromAngles(glm::radians((float)(i + SCALE_FACTOR)), glm::radians((float)(j + SCALE_FACTOR))), glm::vec2(1, 1)));

           vertices.push_back(tigl::Vertex::PT(getXYZfromAngles(glm::radians((float)i), glm::radians((float)(j + SCALE_FACTOR))), glm::vec2(0, 1)));

        }
    }

    
    vbo = tigl::createVbo(vertices);
	
}



void Planetoid::draw()
{

    glm::mat4 originalModelMatrix = tigl::shader->getModelMatrix();
    tigl::shader->setModelMatrix(originalModelMatrix * getModelMatrix());

    glBindTexture(GL_TEXTURE_2D, texId);
    glEnable(GL_TEXTURE_2D);

    tigl::shader->enableTexture(true);
    tigl::drawVertices(GL_QUADS, vbo);
    tigl::shader->enableTexture(false);
	
    tigl::shader->setModelMatrix(originalModelMatrix);

    for (SpaceNode* satellite : satellites)
    {
        satellite->draw();
    }
	
	
}
