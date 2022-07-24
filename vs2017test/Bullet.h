#pragma once
#include "Room.h"
#include "Definitions.h"

class Bullet
{
private:
	double x, y;
	double dirx, diry;
	bool isMoving;

public:
	Bullet();
	Bullet(double x, double y, bool mv, double dx, double dy);

	bool getIsMoving();
	double getX();
	double getY();

	void setX(double x);
	void setY(double y);
	void setDirX(double dx);
	void setDirY(double dy);
	void stopMoving();

	void Shoot();
	void Move(int maze[MSZ][MSZ]);
	void Show();

};
