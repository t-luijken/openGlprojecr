#include "Planetoid.h"

#include <iostream>

#include "stb_image.h"
#include "tigl.h"

#define SCALE_FACTOR 10

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

glm::vec3 calculateNormal(glm::vec3 vect1, glm::vec3 vect2, glm::vec3 vect3)
{
    glm::vec3 U = vect2 - vect1;
    glm::vec3 V = vect3 - vect1;

    float normal_x = (U.y * V.z) - (U.z * V.y);
    float normal_y = (U.z * V.x) - (U.x * V.z);
    float normal_z = (U.x * V.y) - (U.y * V.x);

    return glm::vec3(normal_x, normal_y, normal_z);

	
}
bool contains_vertex_with_same_position(std::vector<tigl::Vertex*>* list, tigl::Vertex* vertexPos)
{
	for (tigl::Vertex* vertex : *list)
	{
		if (vertex->position == vertexPos->position)
		{
            return true;
		}
	}
    return false;
}


Planetoid::Planetoid(std::string name, std::string* textureLink, float rotation_speed, glm::vec3 scale, bool isSun) : SpaceNode(name,rotation_speed, scale)
{
    this->is_sun = isSun;

    glGenTextures(1, &texId);
    glBindTexture(GL_TEXTURE_2D, texId);
	
	
    int width, height, bpp;

    char buffer[128];
    strcpy_s(buffer, sizeof(*textureLink), (*textureLink).c_str());
	
	
    char* imgData =(char*) stbi_load(buffer, &width, &height, &bpp, 4);
	
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


  
	
    for (int longitude = 0; longitude < 180; longitude += SCALE_FACTOR)
    {
        float longRadians = glm::radians((float)longitude);

        float longRadNext = longRadians + glm::radians((float)SCALE_FACTOR);

        float iColorA = (longitude) / 180.0f;
        float iColorB = (longitude + SCALE_FACTOR) / 180.0f;



    	
     

    	
        for (int latitude = -180; latitude < 180; latitude += SCALE_FACTOR)
        {

            float jColorA = (latitude + 180) / 360.0f;
            float jColorB = (latitude + 180 + SCALE_FACTOR) / 360.0f;
        	
            float latRadians = glm::radians((float)latitude);

            float latRadNext = latRadians + glm::radians((float)SCALE_FACTOR);
        	
            float x = sin(longRadians) * cos(latRadians);
            float y = sin(longRadians) * sin(latRadians);
            float z = cos(longRadians);

            float x_next_long = sin(longRadNext) * cos(latRadians);
            float y_next_long = sin(longRadNext) * sin(latRadians);
            float z_next_long = cos(longRadNext);

            float x_next_lat = sin(longRadians) * cos(latRadNext);
            float y_next_lat = sin(longRadians) * sin(latRadNext);
            float z_next_lat = cos(longRadians);

            float x_next_both = sin(longRadNext) * cos(latRadNext);
            float y_next_both = sin(longRadNext) * sin(latRadNext);
            float z_next_both = cos(longRadNext);

        	
            float Rand_red = ((float)(rand() % 255)) / 255.0f;

            float Rand_green = ((float)(rand() % 255)) / 255.0f;

            float Rand_blue = ((float)(rand() % 255)) / 255.0f;


                glm::vec3 normal = calculateNormal(glm::vec3(x, z, y), glm::vec3(x_next_long, z_next_long, y_next_long), glm::vec3(x_next_lat, z_next_lat, y_next_lat));

        		

                normal = normal * glm::vec3(-1, -1, -1);

                

                vertices.push_back(tigl::Vertex::PTN(glm::vec3(x, z, y), glm::vec2(jColorA, 1 - iColorA), normal));
              
                vertices.push_back(tigl::Vertex::PTN(glm::vec3(x_next_long, z_next_long, y_next_long), glm::vec2(jColorA, 1 - iColorB), normal));

                vertices.push_back(tigl::Vertex::PTN(glm::vec3(x_next_lat, z_next_lat, y_next_lat), glm::vec2(jColorB, 1 - iColorA), normal));


                normal = calculateNormal(glm::vec3(x_next_both, z_next_both, y_next_both), glm::vec3(x_next_long, z_next_long, y_next_long), glm::vec3(x_next_lat, z_next_lat, y_next_lat));

        		

        	
                  //  normal = normal * glm::vec3(-1, -1, -1);
                


                vertices.push_back(tigl::Vertex::PTN(glm::vec3(x_next_long, z_next_long, y_next_long), glm::vec2(jColorA, 1 - iColorB), normal));

                vertices.push_back(tigl::Vertex::PTN(glm::vec3(x_next_lat, z_next_lat, y_next_lat), glm::vec2(jColorB, 1 - iColorA), normal));

                vertices.push_back(tigl::Vertex::PTN(glm::vec3(x_next_both, z_next_both, y_next_both), glm::vec2(jColorB, 1 - iColorB), normal));
        }
    }

    std::vector<tigl::Vertex*> calculated;

	
    for (int i = 0; i < vertices.size(); ++i)
    {
	    if (!contains_vertex_with_same_position(&calculated,&vertices[i]))
	    {
            glm::vec3 position = vertices[i].position;
            calculated.push_back(&vertices[i]);

            std::vector<tigl::Vertex*> samePos;
            samePos.push_back(&vertices[i]);

            int total = 1;

            glm::vec3 newNormal(0.0f, 0.0f, 0.0f);

            newNormal + vertices[i].normal;

	    	
            for (int j = 0; j < vertices.size(); ++j)
            {
	            if ((i != j)&&(vertices[j].position==position))
	            {
	            	
                    samePos.push_back(&vertices[j]);
                    newNormal += vertices[j].position;
                    total++;
	            }
            }
	    	
            newNormal /= glm::vec3(total,total,total);

            for (int j = 0; j < samePos.size(); ++j)
            {
                samePos[j]->normal = newNormal;
            }
 	
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

    if (is_sun)
    {
        tigl::shader->enableLighting(false);
    }
	
    tigl::shader->enableTexture(true);
    tigl::drawVertices(GL_TRIANGLES, vbo);
    tigl::shader->enableTexture(false);

    tigl::shader->enableLighting(true);

    



    tigl::shader->setModelMatrix(originalModelMatrix);

    for (SpaceNode* satellite : satellites)
    {
        satellite->draw();
    }
	
	
}
