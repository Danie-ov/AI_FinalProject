#pragma once
#include "Definitions.h"

class HealthPoints
{
private:
	int x;
	int y;
	int room_number;

public:
	HealthPoints();
	HealthPoints(int lx, int ly, int room_number_location);

	int getX();
	int getY();
	int getRoomNumber();

	void setX(int lx);
	void setY(int ly);
	void setRoomNumber(int room_number_location);
};

