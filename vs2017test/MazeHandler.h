#pragma once

#include "Room.h"
#include "glut.h"
#include <time.h>
#include "Grenade.h"
#include "CompareCells.h"
#include <queue>
#include <vector>
#include <iostream>
#include "Ammo.h"
#include "HealthPoints.h"
#include "Definitions.h"

using namespace std;

enum Direction { TOP = 1, BOTTOM = 2, LEFT = 3, RIGHT = 4 };

class MazeHandler
{
public:
	int maze[MSZ][MSZ] = { 0 };
	double security_map[MSZ][MSZ] = { 0 };
	Room rooms[NUM_ROOMS];

	MazeHandler();

	void initMaze();
	void UpdateSecurityMap();
	double Distance(int x1, int y1, int x2, int y2);
	void DigPath(int index1, int index2);
	void DigPathes();
	void AddObstacles();
	void SetupMaze();
	void ShowMaze();
	
	void AddAmmoStock(Ammo* ammo1, Ammo* ammo2);
	void LocateAmmo(Ammo* ammo);
	void AddHealthPoints(HealthPoints* hp1, HealthPoints* hp2);
	void LocateHealthPoints(HealthPoints* hp);
	void RemoveAmmoPoint(Ammo* ammo1, Ammo* ammo2, int row, int col);
	void RemoveHealthPoint(HealthPoints* hp1, HealthPoints* hp2, int row, int col);
	
	void RestorePath(Cell* pCurrent);
	void BuildPath(Cell* pCurrent);
	void UpdatePQ(priority_queue <Cell*, vector<Cell*>, CompareCells>& pq, Cell* pn);
	void AddNeighbor(Cell* pCurrent, int direction,
		priority_queue <Cell*, vector<Cell*>, CompareCells>& pq, vector <Cell>& gray, vector <Cell>& black, int target_index);
};

