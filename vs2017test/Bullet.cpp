#include "Bullet.h"
#include "glut.h"

Bullet::Bullet()
{
	x = 0;
	y = 0;
	dirx = 1;
	diry = 0;
	isMoving = false;
}

Bullet::Bullet(double bx, double by, bool mv, double dx, double dy)
{
	x = bx;
	y = by;
	isMoving = mv;
	dirx = dx;
	diry = dy;
}

bool Bullet::getIsMoving()
{
	return isMoving;
}

double Bullet::getX()
{
	return x;
}

double Bullet::getY()
{
	return y;
}

void Bullet::setX(double x)
{
	this->x = x;
}

void Bullet::setY(double y)
{
	this->y = y;
}

void Bullet::setDirX(double dx)
{
	dirx = dx;
}

void Bullet::setDirY(double dy)
{
	diry = dy;
}

void Bullet::stopMoving()
{
	isMoving = false;
}

void Bullet::Shoot()
{
	isMoving = true;
}

void Bullet::Move(int maze[MSZ][MSZ])
{
	int row, col;

	if (isMoving)
	{
		row = int(MSZ * (y + 1) / 2);
		col = int(MSZ * (x + 1) / 2);

		if (maze[row][col] == WALL) 
		{
			isMoving = false;
		}
		else 
		{
			x += dirx * SPEED;
			y += diry * SPEED;
		}
	}
}

void Bullet::Show()
{
	glColor3d(0, 0, 0);
	glBegin(GL_POLYGON);
	glVertex2d(x, y + 0.01);
	glVertex2d(x + 0.01, y);
	glVertex2d(x, y - 0.01);
	glVertex2d(x - 0.01, y);
	glEnd();
}