#include "Room.h"



Room::Room()
{
	centerX = MSZ / 2;
	centerY = MSZ / 2;
	width = MSZ / 2;
	height = MSZ / 2;
}

Room::Room(int row, int col, int w, int h)
{
	centerY = row;
	centerX = col;
	width = w;
	height = h;
}

Room::~Room()
{
}

void Room::initRoom(int maze[MSZ][MSZ])
{
	int i, j;

	for (i = centerY - height / 2; i <= centerY + height / 2; i++)
		for (j = centerX - width / 2; j <= centerX + width / 2; j++)
			maze[i][j] = SPACE;
}

void Room::AddObstacleCell(int x, int y)
{
	Cell* pn = new Cell(y, x, nullptr, 0, 0);
	obstacle_cells.push_back(pn);
}

bool Room::Overlap(int w, int h, int cx, int cy)
{
	int hdist, vdist; 

	hdist = abs(cx - this->centerX);
	vdist = abs(cy - this->centerY);

	return this->width / 2 + w / 2 > hdist - 4 && vdist - 4 < this->height / 2 + h / 2;
}
