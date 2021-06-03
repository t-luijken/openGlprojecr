#include "Planetoid.h"

#include "stb_image.h"
#include "tigl.h"

#define SCALE_FACTOR 5

const float PI = (2 * acos(0.0));

std::vector<tigl::Vertex> vertices;

tigl::VBO* vbo;


glm::vec3 getXYZfromAngles(float phi, float theta)
{
    float x = cos(phi) * cos(theta - glm::radians(90.0f));
    float y = cos(phi) * sin(theta - glm::radians(90.0f));
    float z = sin(phi);
    return glm::vec3(x, y, z);

}
GLuint texId;

Planetoid::Planetoid(const char* textureLink,  float distance, float speed, float rotation_speed, glm::vec3 scale) : SpaceNode(speed, distance, rotation_speed, scale)
{

    glGenTextures(1, &texId);
    glBindTexture(GL_TEXTURE_2D, texId);
	
	
    int width, height, bpp;
	
	
    char* imgData =(char*) stbi_load("resources/TEST_TEXTURE1.png", &width, &height, &bpp, 4);
	
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
    glBindTexture(GL_TEXTURE_2D, texId);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_image_free(imgData);

	
    for (int i = -180; i < 180; i += SCALE_FACTOR)
    {
       
    	
        for (int j = 0; j < 180; j += SCALE_FACTOR)
        {

            float Rand_red = ((float)(rand() % 255)) / 255.0f;

            float Rand_green = ((float)(rand() % 255)) / 255.0f;

            float Rand_blue = ((float)(rand() % 255)) / 255.0f;

        	


            vertices.push_back(tigl::Vertex::PC(getXYZfromAngles(glm::radians((float)i), glm::radians((float)j)),                                   glm::vec4((i+180)/360.0f, j/180.0f, 0, 1)));

            vertices.push_back(tigl::Vertex::PC(getXYZfromAngles(glm::radians((float)(i + SCALE_FACTOR)), glm::radians((float)j)),                  glm::vec4((i + 180) / 360.0f, j / 180.0f, 0, 1)));

            vertices.push_back(tigl::Vertex::PC(getXYZfromAngles(glm::radians((float)(i + SCALE_FACTOR)), glm::radians((float)(j + SCALE_FACTOR))), glm::vec4((i + 180) / 360.0f, j / 180.0f, 0, 1)));

            vertices.push_back(tigl::Vertex::PC(getXYZfromAngles(glm::radians((float)i), glm::radians((float)(j + SCALE_FACTOR))),                  glm::vec4((i + 180) / 360.0f, j / 180.0f, 0, 1)));


      /*
        float texture_step_height = 1 / 360;
        float texture_step_width = 1/180;
           
        vertices.push_back(tigl::Vertex::PT(getXYZfromAngles(glm::radians((float)i), glm::radians((float)j)),                                    glm::vec2(i * texture_step_height, j * texture_step_width)));

        vertices.push_back(tigl::Vertex::PT(getXYZfromAngles(glm::radians((float)(i + SCALE_FACTOR)), glm::radians((float)j)),                   glm::vec2((i+1) * texture_step_height, j * texture_step_width)));

        vertices.push_back(tigl::Vertex::PT(getXYZfromAngles(glm::radians((float)(i + SCALE_FACTOR)), glm::radians((float)(j + SCALE_FACTOR))),  glm::vec2((i + 1) * texture_step_height, (j+1) * texture_step_width)));

        vertices.push_back(tigl::Vertex::PT(getXYZfromAngles(glm::radians((float)i), glm::radians((float)(j + SCALE_FACTOR))),                   glm::vec2(i * texture_step_height, (j + 1) * texture_step_width)));*/

      /*      float h_offset = 1 / 360;
            float w_offset = 1 / 180;

            vertices.push_back(tigl::Vertex::PT(getXYZfromAngles(glm::radians((float)i), glm::radians((float)j)), glm::vec2(1, 0)));

            vertices.push_back(tigl::Vertex::PT(getXYZfromAngles(glm::radians((float)(i + SCALE_FACTOR)), glm::radians((float)j)), glm::vec2(1, 1)));

            vertices.push_back(tigl::Vertex::PT(getXYZfromAngles(glm::radians((float)(i + SCALE_FACTOR)), glm::radians((float)(j + SCALE_FACTOR))), glm::vec2(0, 1)));

            vertices.push_back(tigl::Vertex::PT(getXYZfromAngles(glm::radians((float)i), glm::radians((float)(j + SCALE_FACTOR))), glm::vec2(0, 0)));*/


        	
          /*  float h_offset = (1.0f / (180.0f/SCALE_FACTOR))/4;
            float w_offset = (1.0f / (360.0f/SCALE_FACTOR))/4;

            float step_size = 1.0f / 144.0f;
        	
        	

            vertices.push_back(tigl::Vertex::PT(getXYZfromAngles(glm::radians((float)i),glm::radians((float)j)),                                    glm::vec2((i * step_size), (j * step_size) )));

            vertices.push_back(tigl::Vertex::PT(getXYZfromAngles(glm::radians((float)(i + SCALE_FACTOR)),glm::radians((float)j)),                   glm::vec2((i * step_size)+step_size, j * w_offset)));  

            vertices.push_back(tigl::Vertex::PT(getXYZfromAngles(glm::radians((float)(i + SCALE_FACTOR)), glm::radians((float)(j + SCALE_FACTOR))), glm::vec2((i * step_size) + step_size, j * step_size)));
        	
            vertices.push_back(tigl::Vertex::PT(getXYZfromAngles(glm::radians((float)i), glm::radians((float)(j + SCALE_FACTOR))),                  glm::vec2((i * step_size), (j * step_size) + step_size)));*/
            

        }
    }

    
    vbo = tigl::createVbo(vertices);
	
}



void Planetoid::draw()
{

    glm::mat4 originalModelMatrix = tigl::shader->getModelMatrix();
    tigl::shader->setModelMatrix(originalModelMatrix * getModelMatrix());

   // glBindTexture(GL_TEXTURE_2D, texId);
    //glEnable(GL_TEXTURE_2D);

   // tigl::shader->enableTexture(true);
    tigl::drawVertices(GL_QUADS, vbo);
   // tigl::shader->enableTexture(false);
	
    tigl::shader->enableLighting(true);
    tigl::shader->setLightCount(1);

    tigl::shader->setLightDirectional(0, false);
    tigl::shader->setLightPosition(0, glm::vec3(1, 2, 3));
    tigl::shader->setLightAmbient(0, glm::vec3(0.1f, 0.1f, 0.15f));
    tigl::shader->setLightDiffuse(0, glm::vec3(0.8f, 0.8f, 0.8f));
    tigl::shader->setLightSpecular(0, glm::vec3(0, 0, 0));
    tigl::shader->setShinyness(32.0f);


    tigl::shader->setModelMatrix(originalModelMatrix);

    for (SpaceNode* satellite : satellites)
    {
        satellite->draw();
    }
	
	
}
