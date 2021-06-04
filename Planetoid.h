#pragma once
#include <string>
#include <GL/glew.h>

#include "SpaceNode.hpp"

class Planetoid : public SpaceNode
{
	bool is_sun;
	GLuint texId;
public:
	Planetoid(std::string* textureLink, float rotation_speed, glm::vec3 scale, bool isSun);

	void draw() override;
	
};
