#pragma once
#include <vector>
#include "Bullet.h"
#include "Definitions.h"

class Grenade
{
private:
	double  x, y;
	Bullet bullets[NUM_BULLETS];
	double dirx, diry;
	bool isExploded;
	int targetx, targety;

public:
	Grenade();
	Grenade(double x, double y);

	double getX();
	double getY();
	bool getIsExploded();
	Bullet* GetBulletFromIndex(int index);
	int getTargetX();
	int getTargetY();

	void setX(double x);
	void setY(double y);
	void setDirX(double dx);
	void setDirY(double dy);
	void setTargetX(int tx);
	void setTargetY(int ty);

	void Throw(int maze[MSZ][MSZ], int soldierX, int soldierY, int x, int y);
	void MoveGrenade();
	void Explode();
	void Exploding(int maze[MSZ][MSZ]);
	void Show();
	void SimulateSecurityMap(int maze[MSZ][MSZ], double map[MSZ][MSZ]);

};
