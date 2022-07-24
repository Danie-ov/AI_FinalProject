#pragma once
#include "Definitions.h"

class Ammo
{
private:
	int x;
	int y;
	int roomNumber;

public:
	Ammo();
	Ammo(int lx, int ly, int roomNumber);

	int getX();
	int getY();
	int getRoomNumber();

	void setX(int x);
	void setY(int y);
	void setRoomNumber(int roomNumber);
};

