#pragma once
#include <string>
#include <GL/glew.h>

#include "SpaceNode.hpp"

class Planetoid : public SpaceNode
{
	bool is_sun;
	bool has_rings;
	GLuint texId;
public:
	Planetoid(std::string name, std::string* textureLink, float rotation_speed, glm::vec3 scale, bool isSun, bool has_rings);

	void draw() override;
	bool* p_get_has_rings();
	bool* p_get_is_sun();
};
