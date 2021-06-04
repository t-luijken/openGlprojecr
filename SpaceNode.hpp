#pragma once
#include <list>
#include <glm/fwd.hpp>
#include <glm/vec3.hpp>

class SpaceNode
{

protected:

	
	SpaceNode* orbit_object;
	glm::vec3 position;
	glm::vec3 rotationAngles;
	glm::vec3 scale;
	std::list<SpaceNode*> satellites;
	float orbit_distance;
	float orbit_speed;
	float rotation_speed;


	float rotation = 0;
	float orbit_distance_traveled = 0;
	

	glm::mat4 getModelMatrix();

public:
	SpaceNode(float rotation_speed, glm::vec3 scale);
	virtual void draw() = 0;
	void update(double timeMillis);
	void add_sat(SpaceNode* sat, float distance, float speed);
	void set_orbit(SpaceNode* node);
	void set_orbit_speed(float rotation_speed);
	void set_distance(float distance);
	
};
