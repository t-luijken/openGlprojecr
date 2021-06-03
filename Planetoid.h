#pragma once
#include <string>

#include "SpaceNode.hpp"

class Planetoid : public SpaceNode
{
public:
	Planetoid(const char* textureLink, float distance, float speed, float rotation_speed, glm::vec3 scale);

	void draw() override;
	
};
