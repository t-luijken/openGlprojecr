#pragma once
#include "ObjModel.h"

namespace spaceShip
{



	void init_ship();
	void draw_ship();
	void update_ship(double timeMillis);

	glm::vec3 getShipPosition();
	glm::vec3 getShipRotation();
	
	void ship_forwards();
	void ship_forwards_release();
	void ship_backwards();
	void ship_backwards_release();
	void ship_left();
	void ship_left_release();
	void ship_right();
	void ship_right_release();
}

