#include "ManSatellite.hpp"

#include "tigl.h"

ManSatellite::ManSatellite(std::string name, std::string model_file_name, float rotation_speed, glm::vec3 scale) : SpaceNode(name, rotation_speed, scale)
{
	this->model = new ObjModel(model_file_name);
}

void ManSatellite::draw()
{
	glm::mat4 originalModelMatrix = tigl::shader->getModelMatrix();
	tigl::shader->setModelMatrix(originalModelMatrix * getModelMatrix());
	this->model->draw();
	tigl::shader->setModelMatrix(originalModelMatrix);

	for (SpaceNode* satellite : satellites)
	{
		satellite->draw();
	}
}