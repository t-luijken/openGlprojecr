#include "SpaceShip.h"

#include <iostream>
#include <glm/ext/matrix_transform.hpp>

#include "Particle.h"

#define MAX_SPEED 50

namespace spaceShip
{
	ObjModel* model;
	glm::vec3 position;
	glm::vec3 rotationAngles;
	glm::vec3 scale;

	float particle_time = 0.1f;
	float timeToNextParticle = 0;

	float roll_angle;
	float roll_speed = 200;

	float max_range = 300;

	Particle* CircularParticleArray[256];
	int arrayPoint = 0;

	bool buttonsPressed[6];

	float rotation_acceleration = 200;
	float rotationSpeed = 0;

	float flight_acceleration = 50;
	float flightSpeed = 0;

	float enginedistance = 2.0f;

	glm::mat4 getModelMatrix()
	{
		glm::mat4 modelMatrix(1.0f);
		modelMatrix = glm::translate(modelMatrix, position);

		modelMatrix = glm::rotate(modelMatrix, glm::radians(rotationAngles[1]), glm::vec3(0, 1, 0));

		modelMatrix = glm::rotate(modelMatrix, glm::radians(rotationAngles[0]), glm::vec3(1, 0, 0));

		modelMatrix = glm::rotate(modelMatrix, glm::radians(rotationAngles[2]), glm::vec3(0, 0, 1));

		modelMatrix = glm::scale(modelMatrix, scale);
		return modelMatrix;
	}
	glm::mat4 getParticleModelMatrix(Particle* particle)
	{
		glm::mat4 modelMatrix(1.0f);
		modelMatrix = glm::translate(modelMatrix, particle->position);
		modelMatrix = glm::rotate(modelMatrix, 0.0f, glm::vec3(1, 0, 0));
		modelMatrix = glm::rotate(modelMatrix, glm::radians(rotationAngles[1]), glm::vec3(0, 1, 0));
		modelMatrix = glm::rotate(modelMatrix, 0.0f, glm::vec3(0, 0, 1));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f * particle->lifetime, 0.1f * particle->lifetime, 0.1f * particle->lifetime));
		return modelMatrix;
	}

	void init_ship()
	{
		model = new ObjModel("models/ship/shipA_OBJ.obj");
		position = glm::vec3(20, 0, 0);
		rotationAngles = glm::vec3(0, 0, 0);
		scale = glm::vec3(0.1f, 0.1f, 0.1f);
	}

	void drawParticle(Particle* particle)
	{
		glm::mat4 originalModelMatrix = tigl::shader->getModelMatrix();
		tigl::shader->setModelMatrix(originalModelMatrix * getParticleModelMatrix(particle));

		assert(particle);

		tigl::begin(GL_TRIANGLES);
		tigl::addVertex(tigl::Vertex::PC(glm::vec3(0, 1, 0), glm::vec4(0, 0, 1, 1)));
		tigl::addVertex(tigl::Vertex::PC(glm::vec3(-1, -1, 0), glm::vec4(0, 0, 1, 1)));
		tigl::addVertex(tigl::Vertex::PC(glm::vec3(1, -1, 0), glm::vec4(0, 0, 1, 1)));
		tigl::end();

		tigl::shader->setModelMatrix(originalModelMatrix);
	}

	void updateParticle(Particle* particle, double timeMillis)
	{
		particle->lifetime -= timeMillis;
		particle->position += particle->direction;
	}

	void draw_ship()
	{
		glm::mat4 originalModelMatrix = tigl::shader->getModelMatrix();
		tigl::shader->setModelMatrix(originalModelMatrix * getModelMatrix());
		model->draw();
		tigl::shader->setModelMatrix(originalModelMatrix);

		float x = sin(glm::radians(rotationAngles[1])) * (flightSpeed);
		float z = cos(glm::radians(rotationAngles[1])) * (flightSpeed);
		glm::vec3 offset(x, 0, z);
		offset += position;

		/*glBegin(GL_LINES);
		glVertex3f(position[0],position[1] ,position[2]);
		glVertex3f(offset[0],offset[1],offset[2]);
		glEnd();*/

		tigl::shader->enableLighting(false);

		for (Particle* particle : CircularParticleArray)
		{
			if (particle)
			{
				if (particle->lifetime > 0)
				{
					drawParticle(particle);
				}
			}
		}

		tigl::shader->enableLighting(true);
	}

	glm::vec3 getShipPosition()
	{
		return position;
	}

	glm::vec3 getShipRotation()
	{
		return rotationAngles;
	}

	void ship_forwards()
	{
		buttonsPressed[0] = true;
		particle_time = 0.001f;
	}

	void ship_forwards_release()
	{
		buttonsPressed[0] = false;
		particle_time = 0.1f;
	}

	void ship_backwards()
	{
		buttonsPressed[1] = true;
	}
	void ship_backwards_release()
	{
		buttonsPressed[1] = false;
	}

	void ship_left()
	{
		buttonsPressed[2] = true;
	}

	void ship_left_release()
	{
		buttonsPressed[2] = false;
	}

	void ship_right()
	{
		buttonsPressed[3] = true;
	}

	void ship_right_release()
	{
		buttonsPressed[3] = false;
	}

	void update_ship(double timeMillis)
	{
		particle_time -= timeMillis;
		if (particle_time <= 0)
		{
			float x = (sin(glm::radians(rotationAngles[1])) / 10.0f) * ((flightSpeed/MAX_SPEED)+0.5);
			float z = (cos(glm::radians(rotationAngles[1])) / 10.0f) * ((flightSpeed/MAX_SPEED)+0.5);
			glm::vec3 direction(x, 0.0005f, z);

			float randX = (((float)(rand() % 10)) - 5.0f) / 10.0f;
			float randY = (((float)(rand() % 10)) - 5.0f) / 10.0f;
			float randZ = (((float)(rand() % 10)) - 5.0f) / 10.0f;

			glm::vec3 offset(randX, randY, randZ);

			float engineX = sin(glm::radians(rotationAngles[1])) * (enginedistance);
			float engineZ = cos(glm::radians(rotationAngles[1])) * (enginedistance);
			float engineY = sin(glm::radians(rotationAngles[0])) * -(enginedistance);
			glm::vec3 engineOffset(engineX, engineY, engineZ);

			glm::vec3 pPosition = position;

			pPosition += offset;
			pPosition += engineOffset;

			delete CircularParticleArray[arrayPoint];

			Particle* particle = new struct Particle(direction, pPosition);

			CircularParticleArray[arrayPoint] = particle;

			arrayPoint++;
			if (arrayPoint == 256)
			{
				arrayPoint = 0;
			}

			if (position.x < -max_range)
			{
				position.x = -max_range;
			}
			if (position.x > max_range)
			{
				position.x = max_range;
			}

			if (position.y < -max_range)
			{
				position.y = -max_range;
			}
			if (position.y > max_range)
			{
				position.y = max_range;
			}

			if (position.z < -max_range)
			{
				position.z = -max_range;
			}
			if (position.z > max_range)
			{
				position.z = max_range;
			}
		}

		if (buttonsPressed[0])
		{
			if (flightSpeed < MAX_SPEED)
			{
				flightSpeed += flight_acceleration * timeMillis;
			}
		}
		if (buttonsPressed[1])
		{
			if (flightSpeed > -MAX_SPEED)
			{
				flightSpeed -= flight_acceleration * timeMillis;
			}
		}

		if (!(buttonsPressed[0]) && !(buttonsPressed[1]))
		{
			if (flightSpeed > 0.05f)
			{
				flightSpeed -= (flight_acceleration / 2.0f) * timeMillis;
			}
			else if (flightSpeed > 0)
			{
				flightSpeed = 0;
			}

			if (flightSpeed < -0.05f)
			{
				flightSpeed += (flight_acceleration / 2.0f) * timeMillis;
			}
			else if (flightSpeed < 0)
			{
				flightSpeed = 0;
			}
		}

		float x = sin(glm::radians(rotationAngles[1])) * (flightSpeed * timeMillis);
		float z = cos(glm::radians(rotationAngles[1])) * (flightSpeed * timeMillis);
		float y = sin(glm::radians(rotationAngles[0])) * (flightSpeed * timeMillis);
		glm::vec3 offset(x, -y, z);

		position -= offset;

		if (buttonsPressed[2])
		{
			if (rotationSpeed < 200)
			{
				rotationSpeed += rotation_acceleration * timeMillis;
			}

			if (rotationAngles[2] < 90)
			{
				rotationAngles[2] += roll_speed * timeMillis;
			}
		}
		if (buttonsPressed[3])
		{
			if (rotationSpeed > -200)
			{
				rotationSpeed -= rotation_acceleration * timeMillis;
			}

			if (rotationAngles[2] > -90)
			{
				rotationAngles[2] -= roll_speed * timeMillis;
			}
		}

		rotationAngles[1] += rotationSpeed * timeMillis;

		if (!(buttonsPressed[2]) && !(buttonsPressed[3]))
		{
			if (rotationSpeed > 1.0f)
			{
				rotationSpeed -= (rotation_acceleration / 2.0f) * timeMillis;
			}
			else if (rotationSpeed > 0)
			{
				rotationSpeed = 0;
			}
			if (rotationSpeed < -1.0f)
			{
				rotationSpeed += (rotation_acceleration / 2.0f) * timeMillis;
			}
			else if (rotationSpeed < 0)
			{
				rotationSpeed = 0;
			}

			if (rotationAngles[2] > 0.1f)
			{
				rotationAngles[2] -= (roll_speed / 2.0f) * timeMillis;
			}
			else if (rotationAngles[2] > 0)
			{
				rotationAngles[2] = 0;
			}
			if (rotationAngles[2] < -0.1f)
			{
				rotationAngles[2] += (roll_speed / 2.0f) * timeMillis;
			}
			else if (rotationAngles[2] < 0)
			{
				rotationAngles[2] = 0;
			}
		}

		if (buttonsPressed[4])
		{
			if (rotationAngles[0] < 45)
			{
				rotationAngles[0] += roll_speed * timeMillis;
			}
		}

		if (buttonsPressed[5])
		{
			if (rotationAngles[0] > -45)
			{
				rotationAngles[0] -= roll_speed * timeMillis;
			}
		}

		if (!(buttonsPressed[4]) && !(buttonsPressed[5]))
		{
			if (rotationAngles[0] > 0.05f)
			{
				rotationAngles[0] -= (roll_speed / 2.0f) * timeMillis;
			}
			else if (rotationAngles[0] > 0)
			{
				rotationAngles[0] = 0;
			}
			if (rotationAngles[0] < -0.05f)
			{
				rotationAngles[0] += (roll_speed / 2.0f) * timeMillis;
			}
			else if (rotationAngles[0] < 0)
			{
				rotationAngles[0] = 0;
			}
		}

		for (Particle* particle : CircularParticleArray)
		{
			if (particle)
			{
				if (particle->lifetime > 0)
				{
					updateParticle(particle, timeMillis);
				}
			}
		}

		if (rotationAngles[1] > 360.0f)
		{
			rotationAngles[1] -= 360.0f;
		}
	}
}

void spaceShip::on_press_shift()
{
	buttonsPressed[4] = true;
}

void spaceShip::on_release_shift()
{
	buttonsPressed[4] = false;
}

void spaceShip::on_press_control()
{
	buttonsPressed[5] = true;
}

void spaceShip::on_release_control()
{
	buttonsPressed[5] = false;
}