#include "HealthPoints.h"

HealthPoints::HealthPoints()
{
	x = 0;
	y = 0;
	room_number = 0;
}

HealthPoints::HealthPoints(int lx, int ly, int room_number_location)
{
	x = lx;
	y = ly;
	room_number = room_number_location;
}

int HealthPoints::getX()
{
	return x;
}

int HealthPoints::getY()
{
	return y;
}

int HealthPoints::getRoomNumber()
{
	return room_number;
}

void HealthPoints::setX(int lx)
{
	x = lx;
}

void HealthPoints::setY(int ly)
{
	y = ly;
}

void HealthPoints::setRoomNumber(int room_number_location)
{
	room_number = room_number_location;
}