#include "Ammo.h"

Ammo::Ammo()
{
	x = 0;
	y = 0;
	roomNumber = 0;
}

Ammo::Ammo(int lx, int ly, int room_number)
{
	x = lx;
	y = ly;
	roomNumber = room_number;
}

int Ammo::getX()
{
	return x;
}

int Ammo::getY()
{
	return y;
}

int Ammo::getRoomNumber()
{
	return roomNumber;
}

void Ammo::setX(int lx)
{
	x = lx;
}

void Ammo::setY(int ly)
{
	y = ly;
}

void Ammo::setRoomNumber(int room_number)
{
	roomNumber = room_number;
}