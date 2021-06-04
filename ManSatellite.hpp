#pragma once
#include "SpaceNode.hpp"
#include "ObjModel.h"

class ManSatellite : public SpaceNode
{
private:
	 ObjModel* model;
public:
	ManSatellite(std::string model_file_name, float rotation_speed, glm::vec3 scale);
	
	void draw() override;

};
