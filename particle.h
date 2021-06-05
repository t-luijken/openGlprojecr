#pragma once
#include <glm/vec3.hpp>

struct Particle
{
	float lifetime;
	glm::vec3 direction;
	glm::vec3 position;
	Particle(glm::vec3 direction, glm::vec3 position)
	{
		this->direction = direction;
		this->position = position;
		lifetime = 1.0f;
	}
};
