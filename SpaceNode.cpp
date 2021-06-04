#include "SpaceNode.hpp"

#include <iostream>
#include <glm/ext/matrix_transform.hpp>


SpaceNode::SpaceNode(std::string name,  float rotation_speed, glm::vec3 scale)
{
	this->name = name;

	this->rotation_speed = rotation_speed;
	this->scale = scale;
	

	this->rotationAngles = glm::vec3(0, 0, 0);

	if (!orbit_object)
	{
		position = glm::vec3(0, 0, 0);
	}else
	{
		float x_offset = orbit_distance * sin(glm::radians(orbit_distance_traveled));
		float y_offset = orbit_distance * cos(glm::radians(orbit_distance_traveled));
		glm::vec3 offset(x_offset, y_offset, 0);
	    

		this->position = orbit_object->position + offset;
	}
	
}


void SpaceNode::update(double timeMillis)
{
	orbit_distance_traveled += orbit_speed * timeMillis;
	rotation += rotation_speed * timeMillis;

	rotation = fmod(rotation, 360);
	rotationAngles[1] = glm::radians(rotation);


	for (SpaceNode* satellite : satellites)
	{
		satellite->update(timeMillis);
	}
	if (orbit_object)
	{
		float x_offset = orbit_distance * sin(glm::radians(orbit_distance_traveled));
		float y_offset = orbit_distance * cos(glm::radians(orbit_distance_traveled));
		glm::vec3 offset(x_offset, 0 ,y_offset );

		this->position = orbit_object->position + offset;
	}
	
}

void SpaceNode::add_sat(SpaceNode* sat,float distance,float speed)
{
	satellites.push_back(sat);
	sat->set_orbit(this);
	sat->set_distance(distance);
	sat->set_orbit_speed(speed);
}

void SpaceNode::set_orbit(SpaceNode* node)
{
	orbit_object = node;
}

glm::mat4 SpaceNode::getModelMatrix()
{
	glm::mat4 modelMatrix(1.0f);
	modelMatrix = glm::translate(modelMatrix, this->position);
	modelMatrix = glm::rotate(modelMatrix, this->rotationAngles[0], glm::vec3(1, 0, 0));
	modelMatrix = glm::rotate(modelMatrix, this->rotationAngles[1], glm::vec3(0, 1, 0));
	modelMatrix = glm::rotate(modelMatrix, this->rotationAngles[2], glm::vec3(0, 0, 1));
	modelMatrix = glm::scale(modelMatrix, this->scale);
	return modelMatrix;
}

void SpaceNode::set_orbit_speed(float rotation_speed)
{
	this->orbit_speed = rotation_speed;
}
void SpaceNode::set_distance(float distance)
{
	this->orbit_distance = distance;
}


std::string SpaceNode::get_name()
{
	return this->name;
}

glm::vec3 SpaceNode::get_position()
{
	return position;
}

glm::vec3 SpaceNode::get_rotation()
{
	return rotationAngles;
}
