#include "Grenade.h"
#include "glut.h"
#include <math.h>

Grenade::Grenade()
{
	x = 0;
	y = 0;
	targetx = 0;
	targety = 0;
	dirx = 1;
	diry = 0;
	isExploded = false;
	int i;
	double alpha, teta = 2 * PI / NUM_BULLETS;
	for (i = 0, alpha = 0; i < NUM_BULLETS; i++, alpha += teta)
	{
		bullets[i].setX(x);
		bullets[i].setY(y);
		bullets[i].setDirX(cos(alpha));
		bullets[i].setDirY(sin(alpha));
	}
}

Grenade::Grenade(double x, double y)
{
	double alpha, teta = 2 * PI / NUM_BULLETS;
	int i;
	this->x = x;
	this->y = y;
	targetx = 0;
	targety = 0;
	dirx = 1;
	diry = 0;
	isExploded = false;
	for (i = 0, alpha = 0; i < NUM_BULLETS; i++, alpha += teta)
	{
		bullets[i].setX(x);
		bullets[i].setY(y);
		bullets[i].setDirX(cos(alpha));
		bullets[i].setDirY(sin(alpha));
	}
}

double Grenade::getX()
{
	return  MSZ * (x + 1) / 2;
}

double Grenade::getY()
{
	return  MSZ * (y + 1) / 2;
}

bool Grenade::getIsExploded()
{
	int i;
	for (i = 0; i < NUM_BULLETS; i++)
	{
		if (bullets[i].getIsMoving())
			return true;
	}

	targetx = 0;
	targety = 0;
	return false;
}

Bullet* Grenade::GetBulletFromIndex(int index)
{
	return &bullets[index];
}

int Grenade::getTargetX()
{
	return targetx;
}

int Grenade::getTargetY()
{
	return targety;
}

void Grenade::setX(double lx)
{

	x = lx;
	double alpha, teta = 2 * PI / NUM_BULLETS;
	int i;

	for (i = 0, alpha = 0; i < NUM_BULLETS; i++, alpha += teta)
	{
		bullets[i].setX(x);
		bullets[i].setDirX(cos(alpha));
	}

}

void Grenade::setY(double ly)
{
	y = ly;
	double alpha, teta = 2 * PI / NUM_BULLETS;
	int i;

	for (i = 0, alpha = 0; i < NUM_BULLETS; i++, alpha += teta)
	{
		bullets[i].setY(x);
		bullets[i].setDirY(sin(alpha));
	}
}

void Grenade::setDirX(double dx)
{
	dirx = dx;
}

void Grenade::setDirY(double dy)
{
	diry = dy;
}

void Grenade::setTargetX(int tx)
{
	targetx = tx;
}

void Grenade::setTargetY(int ty)
{
	targety = ty;
}

void Grenade::Throw(int maze[MSZ][MSZ], int soldierX, int soldierY, int opponentX, int opponentY)
{
	double xdistance, ydistance, size;
	int i = 0;
	int randx = 0, randy = 0;

	if (targetx == 0 && targety == 0) 
	{
		targetx = opponentX;
		targety = opponentY;
		// Check Up
		if (maze[opponentY + 1][opponentX] == SPACE)
			targety += 1;
		// Check Right
		else if (maze[opponentY][opponentX + 1] == SPACE)
			targetx += 1;
		// Check Left
		else if (maze[opponentY][opponentX - 1] == SPACE)
			targetx -= 1;
		// Check Up - Right
		else if (maze[opponentY + 1][opponentX + 1] == SPACE)
		{
			targetx += 1;
			targety += 1;
		}
		// Check Up - Left
		else if (maze[opponentY + 1][opponentX - 1] == SPACE)
		{
			targetx -= 1;
			targety += 1;
		}
		// Check Down
		else if (maze[opponentY - 1][opponentX] == SPACE)
			targety -= 1;
		// Check Down - Right
		else if (maze[opponentY - 1][opponentX + 1] == SPACE)
		{
			targetx += 1;
			targety += 1;
		}
		// Check Down - Left
		else if (maze[opponentY - 1][opponentX - 1] == SPACE)
		{
			targetx -= 1;
			targety -= 1;
		}
		isExploded = true;

		xdistance = (double)targetx - (double)soldierX;
		ydistance = (double)targety - (double)soldierY;
		size = sqrt(pow(xdistance, 2) + pow(ydistance, 2));
		dirx = xdistance / size;
		diry = ydistance / size;
	}

	while (i < GRENADE_MOVEMENT && isExploded)
	{
		MoveGrenade();
		i++;
	}
	Show();
}

void Grenade::MoveGrenade()
{
	int row, col;

	if (isExploded)
	{
		row = int(MSZ * (y + 1) / 2);
		col = int(MSZ * (x + 1) / 2);

		if (row == targety && col == targetx) 
		{
			isExploded = false;
		}
		else
		{
			if (row == targety)
				x += dirx * SPEED;
			else if (col == targetx)
				y += diry * SPEED;
			else
			{
				x += dirx * SPEED;
				y += diry * SPEED;
			}

			for (int i = 0; i < NUM_BULLETS; i++)
			{
				bullets[i].setX(x);
				bullets[i].setY(y);
			}
		}
	}
}

void Grenade::Explode()
{
	double alpha, teta = 2 * PI / NUM_BULLETS;
	int i;
	for (i = 0, alpha = 0; i < NUM_BULLETS; i++, alpha += teta)
	{
		bullets[i].setDirX(cos(alpha));
		bullets[i].setDirY(sin(alpha));
		bullets[i].Shoot();
	}
}

void Grenade::Exploding(int maze[MSZ][MSZ])
{
	for (int i = 0; i < NUM_BULLETS; i++)
		bullets[i].Move(maze);

}

void Grenade::Show()
{
	for (int i = 0; i < NUM_BULLETS; i++)
		bullets[i].Show();
}

void Grenade::SimulateSecurityMap(int maze[MSZ][MSZ], double map[MSZ][MSZ])
{
	bool stop = false;
	int row, col, i;
	double delta = 0.00025;
	for (i = 0; i < NUM_BULLETS; i++)
		bullets[i].Shoot();

	while (!stop)
	{
		stop = true;
		for (i = 0; i < NUM_BULLETS; i++)
		{
			if (bullets[i].getIsMoving())
			{
				stop = false;
				row = int(MSZ * (bullets[i].getY() + 1) / 2);
				col = int(MSZ * (bullets[i].getX() + 1) / 2);
				if (row >= 0 && row < MSZ && col >= 0 && col < MSZ &&
					(maze[row][col] == TEAM_1 || maze[row][col] == TEAM_2 || maze[row][col] == SPACE))
				{
					map[row][col] += delta;
					bullets[i].Move(maze);
				}
				else
					bullets[i].stopMoving();
			}
		}
	}
}
