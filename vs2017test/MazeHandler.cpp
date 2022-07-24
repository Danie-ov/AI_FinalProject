#include "MazeHandler.h"

MazeHandler::MazeHandler()
{
	initMaze();
}

void MazeHandler::initMaze()
{
	glClearColor(0, 0, 0.4, 0);

	srand(time(0));
	glOrtho(-1, 1, -1, 1, -1, 1);

	SetupMaze();
}

void MazeHandler::UpdateSecurityMap()
{
	int num_grenades = 30000;
	int i;
	double x, y;
	
	for (i = 0; i < num_grenades; i++)
	{
		x = 2 * (double)(rand() % MSZ) / (double)MSZ - 1;
		y = 2 * (double)(rand() % MSZ) / (double)MSZ - 1;

		Grenade* p = new Grenade(x, y);
		p->SimulateSecurityMap(maze, security_map);
	}
}

double MazeHandler::Distance(int x1, int y1, int x2, int y2)
{
	return sqrt(pow((x1 - x2), 2) + pow(y1 - y2, 2));
}

void MazeHandler::UpdatePQ(priority_queue<Cell*, vector<Cell*>, CompareCells>& pq, Cell* pn)
{
	bool found = false;
	Cell* ptmp;
	vector <Cell*> tmp_v;

	while (!pq.empty() && !found)
	{
		ptmp = pq.top();
		pq.pop();
		if ((*ptmp).operator==(*pn))
		{
			found = true;
			pq.push(pn);
		}
		else
			tmp_v.push_back(ptmp);
	}

	while (!tmp_v.empty())
	{
		ptmp = tmp_v.back();
		tmp_v.pop_back();
		pq.push(ptmp);
	}
}

void MazeHandler::AddNeighbor(Cell* pCurrent, int direction, priority_queue<Cell*, vector<Cell*>,
										CompareCells>& pq, vector<Cell>& gray, vector<Cell>& black, int target_index)
{
	int i = 0, j = 0;
	Cell* pn;
	double g, h;
	vector<Cell>::iterator it_gray;
	vector<Cell>::iterator it_black;

	switch (direction)
	{
	case TOP:
		i = pCurrent->getRow() + 1;
		j = pCurrent->getCol();
		break;
	case BOTTOM:
		i = pCurrent->getRow() - 1;
		j = pCurrent->getCol();
		break;
	case LEFT:
		i = pCurrent->getRow();
		j = pCurrent->getCol() - 1;
		break;
	case RIGHT:
		i = pCurrent->getRow();
		j = pCurrent->getCol() + 1;
		break;
	}
	h = Distance(j, i, rooms[target_index].getCenterX(), rooms[target_index].getCenterY());
	if (maze[i][j] == WALL)
		g = pCurrent->getG() + WALL_COST;
	else 
		g = pCurrent->getG() + SPACE_COST;

	pn = new Cell(i, j, pCurrent, g, h);
	it_black = find(black.begin(), black.end(), *pn); 
	if (it_black != black.end()) 
	{
		delete pn;
		return;
	}
	else  
	{
		it_gray = find(gray.begin(), gray.end(), *pn);
		if (it_gray == gray.end()) 
		{
			pq.push(pn);
			gray.push_back(*pn);
		}
		else 
		{
			if (it_gray->getF() < pn->getF()) 
			{
				delete pn;
				return;
			}
			else 
			{
				UpdatePQ(pq, pn);
				gray.erase(it_gray);
				gray.push_back(*pn);
			}
		}
	}
}

void MazeHandler::RestorePath(Cell* pCurrent)
{
	while (maze[pCurrent->getRow()][pCurrent->getCol()] != START)
	{
		maze[pCurrent->getRow()][pCurrent->getCol()] = SPACE;
		pCurrent = pCurrent->getParent();
	}
	maze[pCurrent->getRow()][pCurrent->getCol()] = SPACE;
}

void MazeHandler::DigPath(int index1, int index2)
{
	vector <Cell> gray;
	vector <Cell> black;
	priority_queue <Cell*, vector<Cell*>, CompareCells> pq;
	vector<Cell>::iterator it_gray;
	vector<Cell>::iterator it_black;

	int is, js, it, jt;
	Cell* pn;
	Cell* pCurrent;
	is = rooms[index1].getCenterY();
	js = rooms[index1].getCenterX();
	it = rooms[index2].getCenterY();
	jt = rooms[index2].getCenterX();
	maze[is][js] = START;
	maze[it][jt] = TARGET;
	pn = new Cell(is, js, nullptr, 0, Distance(js, is, jt, it));
	pq.push(pn);
	gray.push_back(*pn);
	while (!pq.empty())
	{
		pCurrent = pq.top();
		pq.pop(); 
		if (maze[pCurrent->getRow()][pCurrent->getCol()] == TARGET)
		{
			RestorePath(pCurrent);
			return; // 
		}
		else 
		{
			black.push_back(*pCurrent);
			it_gray = find(gray.begin(), gray.end(), *pCurrent); 
			if (it_gray != gray.end()) 
				gray.erase(it_gray);
				// check top neighbor
			if (pCurrent->getRow() + 1 < MSZ)
				AddNeighbor(pCurrent, TOP, pq, gray, black, index2);
			// check left neighbor
			if (pCurrent->getCol() - 1 >= 0)
				AddNeighbor(pCurrent, LEFT, pq, gray, black, index2);
			// check right neighbor
			if (pCurrent->getCol() + 1 < MSZ)
				AddNeighbor(pCurrent, RIGHT, pq, gray, black, index2);
			// check bottom neighbor
			if (pCurrent->getRow() - 1 >= 0)
				AddNeighbor(pCurrent, BOTTOM, pq, gray, black, index2);
		}
	}
	cout << "Priority queue is empty. The target was missed\n";
}

void MazeHandler::DigPathes()
{
	int i, j;

	for (i = 0; i < NUM_ROOMS; i++)
		for (j = i + 1; j < NUM_ROOMS; j++)
			DigPath(i, j);
}

void MazeHandler::SetupMaze()
{
	int i, j, k;
	int countRoom;
	int w, h, cx, cy;

	for (i = 0; i < MSZ; i++)
		for (j = 0; j < MSZ; j++)
			maze[i][j] = WALL;

	for (countRoom = 0; countRoom < NUM_ROOMS; countRoom++)
	{
		bool hasOverlap;
		do
		{
			hasOverlap = false;

			w = 7 + rand() % (MSZ / 5);
			h = 7 + rand() % (MSZ / 5);
			cx = 1 + w / 2 + rand() % (MSZ - w - 2);
			cy = 1 + h / 2 + rand() % (MSZ - h - 2);

			for (k = 0; k < countRoom && !hasOverlap; k++)
				if (rooms[k].Overlap(w, h, cx, cy))
					hasOverlap = true;
		} while (hasOverlap);
		rooms[countRoom].setWidth(w);
		rooms[countRoom].setHeight(h);
		rooms[countRoom].setCenterX(cx);
		rooms[countRoom].setCenterY(cy);
		rooms[countRoom].initRoom(maze);
	}

	DigPathes();
	AddObstacles();
}

void MazeHandler::ShowMaze()
{
	int i, j;
	double dx, dy, x, y;

	dx = dy = 2.0 / MSZ; 
	y = -1;
	for (i = 0; i < MSZ; i++, y += dy)
	{
		x = -1;
		for (j = 0; j < MSZ; j++, x += dx)
		{
			double d = security_map[i][j];
			switch (maze[i][j]) {
			case WALL:
				glColor3d(0.5, 0, 0);
				break;
			case SPACE:
				glColor3d(1 - d, 1 - d, 1 - d);
				break;
			case SECURE_POINT:
				glColor3d(1 - d, 1 - d, 1 - d);
				break;
			case START:
				glColor3d(0.2, 0.4, 1);
				break;
			case TARGET:
				glColor3d(1, 0, 0);
				break;
			case BLACK_SPACE:
				glColor3d(1 - d, 1 - d, 1 - d);;
				break;
			case GRAY:
				glColor3d(0, 0.8, 0);
				break;
			case PATH:
				glColor3d(1, 0.5, 1);
				break;
			case AMMO:
				glColor3d(0.5, 0.5, 0);
				break;
			case HEALT_POINTS:
				glColor3d(0.2, 1, 0.2);
				break;
			case TEAM_1:
				glColor3d(0, 1, 1); //cyan team
				break;
			case TEAM_2:
				glColor3d(1, 0.4, 0.7); //pink team
				break;
			}
			
			glBegin(GL_POLYGON);
			glVertex2d(x, y);
			glVertex2d(x, y + dy);
			glVertex2d(x + dx, y + dy);
			glVertex2d(x + dx, y);
			glEnd();
		}
	}
}

void MazeHandler::BuildPath(Cell* pCurrent)
{
	while (maze[pCurrent->getRow()][pCurrent->getCol()] != START)
	{
		maze[pCurrent->getRow()][pCurrent->getCol()] = PATH;
		pCurrent = pCurrent->getParent();
	}
}

void MazeHandler::AddObstacles()
{
	const int up = 0, down = 1, right = 2, left = 3;
	int obstacle_size, num_obstacles, currentX, currentY, minX, minY;
	int side, i, j, k;
	for (i = 0; i < NUM_ROOMS; i++)
	{
		num_obstacles = rand() % 2 + 1;
		for (k = 0; k < num_obstacles; k++)
		{
			j = 0;
			obstacle_size = rand() % 8 + 2; 
			minX = rooms[i].getCenterX() - rooms[i].getWidth() / 2;
			minY = rooms[i].getCenterY() - rooms[i].getHeight() / 2;
			currentX = rand() % (rooms[i].getWidth() - 4) + (minX + 1);
			currentY = rand() % (rooms[i].getHeight() - 4) + (minY + 1);
			while (j < obstacle_size)
			{
				maze[currentY][currentX] = WALL;
				rooms[i].AddObstacleCell(currentX, currentY);
				side = rand() % 4;
				if (side == up && currentY + 1 < minY + rooms[i].getHeight())
					currentY += 1;
				else if (side == down && currentY - 1 > minY)
					currentY -= 1;
				else if (side == right && currentX + 1 < minX + rooms[i].getWidth())
					currentX += 1;
				else if (side == left && currentX - 1 < minX)
					currentX -= 1;
				j++;
			}
		}
	}
}

void MazeHandler::AddAmmoStock(Ammo* ammo1, Ammo* ammo2)
{
	ammo1->setRoomNumber(rand() % NUM_ROOMS);
	LocateAmmo(ammo1);
	do
	{
		ammo2->setRoomNumber(rand() % NUM_ROOMS);
	} while (ammo1->getRoomNumber() == ammo2->getRoomNumber());

	LocateAmmo(ammo2);

	maze[ammo1->getY()][ammo1->getX()] = AMMO;
	maze[ammo2->getY()][ammo2->getX()] = AMMO;
}

void MazeHandler::LocateAmmo(Ammo* ammo)
{
	do {
		ammo->setX(rand() % rooms[ammo->getRoomNumber()].getWidth() +
			(rooms[ammo->getRoomNumber()].getCenterX() - rooms[ammo->getRoomNumber()].getWidth() / 2));
		ammo->setY(rand() % rooms[ammo->getRoomNumber()].getHeight() +
			(rooms[ammo->getRoomNumber()].getCenterY() - rooms[ammo->getRoomNumber()].getHeight() / 2));
	} while (maze[ammo->getY()][ammo->getX()] != SPACE);
}

void MazeHandler::AddHealthPoints(HealthPoints* hp1, HealthPoints* hp2)
{
	hp1->setRoomNumber(rand() % NUM_ROOMS);
	LocateHealthPoints(hp1);
	do
	{
		hp2->setRoomNumber(rand() % NUM_ROOMS);
	} while (hp1->getRoomNumber() == hp2->getRoomNumber());

	LocateHealthPoints(hp2);

	maze[hp1->getY()][hp1->getX()] = HEALT_POINTS;
	maze[hp2->getY()][hp2->getX()] = HEALT_POINTS;
}

void MazeHandler::LocateHealthPoints(HealthPoints* hp)
{
	do {
		hp->setX(rand() % rooms[hp->getRoomNumber()].getWidth() +
			(rooms[hp->getRoomNumber()].getCenterX() - rooms[hp->getRoomNumber()].getWidth() / 2));
		hp->setY(rand() % rooms[hp->getRoomNumber()].getHeight() +
			(rooms[hp->getRoomNumber()].getCenterY() - rooms[hp->getRoomNumber()].getHeight() / 2));
	} while (maze[hp->getY()][hp->getX()] != SPACE);
}

void MazeHandler::RemoveAmmoPoint(Ammo* ammo1, Ammo* ammo2, int row, int col)
{
	if (ammo1->getY() == row && ammo1->getX() == col)
	{
		ammo1->setX(RANDOM_VALUE);
		ammo1->setY(RANDOM_VALUE);
	}
	else
	{
		ammo2->setX(RANDOM_VALUE);
		ammo2->setY(RANDOM_VALUE);
	}
}

void MazeHandler::RemoveHealthPoint(HealthPoints* hp1, HealthPoints* hp2, int row, int col)
{
	if (hp1->getY() == row && hp1->getX() == col)
	{
		hp1->setX(RANDOM_VALUE);
		hp1->setY(RANDOM_VALUE);
	}
	else
	{
		hp2->setX(RANDOM_VALUE);
		hp2->setY(RANDOM_VALUE);
	}
}