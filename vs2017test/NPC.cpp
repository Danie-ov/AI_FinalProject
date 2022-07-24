#include "NPC.h"
#include "glut.h"
#include "Attack.h"

using namespace std;

const char* troopTypes[2] = { "Soldier", "Carrier" };

NPC::NPC()
{
	locationX = 0;
	locationY = 0;
	health = START_HEALTH_POINTS;
	/*Soldier*/
	bullets_ammo = START_BULLETS;
	grenade_ammo = START_GRENADES;
	/*Carrier*/
	health_stock = BASKET_HEALTH_POINTS;
	bullets_stock = BASKET_BULLETS;
	grenade_stock = BASKET_GRENADES;
	isInRoom = true;
	room_index = 0;
	color = SPACE;
	start = nullptr;
	target = nullptr;
	pb = nullptr;
	pg = nullptr;
	isSearchingEnemy = false;
	isSearchingAmmoAndHealth = false;
	isSearchingCover = false; 
	isAttacking = true;
	isSuplying = false;

	pCurrentState = new Attack();
	pCurrentState->OnEnter(this);
}

NPC::NPC(int x, int y, int room, int team_color, eType t)
{
	locationX = x;
	locationY = y;
	type = t;
	health = START_HEALTH_POINTS;
	/*Soldier*/
	bullets_ammo = START_BULLETS;
	grenade_ammo = START_GRENADES;
	/*Carrier*/
	health_stock = BASKET_HEALTH_POINTS;
	bullets_stock = BASKET_BULLETS;
	grenade_stock = BASKET_GRENADES;
	isInRoom = true;
	room_index = room;
	color = team_color;
	start = nullptr;
	target = nullptr;
	pb = nullptr;
	pg = nullptr;

	isSearchingEnemy = false;
	isSearchingAmmoAndHealth = false;
	isSearchingCover = false;
	isSuplying = false;
	isAttacking = true;

	pCurrentState = new Attack();
	pCurrentState->OnEnter(this);
}

bool NPC::CheckIfSecure(MazeHandler* maze, NPC opponent)
{
	int minX, minY, maxX, maxY;
	minX = maze->rooms[opponent.getRoomIndex()].getCenterX() - maze->rooms[opponent.getRoomIndex()].getWidth() / 2;
	minY = maze->rooms[opponent.getRoomIndex()].getCenterY() - maze->rooms[opponent.getRoomIndex()].getHeight() / 2;
	maxX = minX + maze->rooms[opponent.getRoomIndex()].getWidth();
	maxY = minY + maze->rooms[opponent.getRoomIndex()].getHeight();

	if ((maze->maze[locationY + 1][locationX] == WALL && locationY + 1 < maxY) ||
		(maze->maze[locationY - 1][locationX] == WALL && locationY - 1 > minY) ||
		(maze->maze[locationY][locationX + 1] == WALL && locationX + 1 < maxX) ||
		(maze->maze[locationY][locationX - 1] == WALL && locationX - 1 > minX))
		return true;

	return false;
}

void NPC::FindTargetPoint(int x1, int y1, int x2, int y2)
{
	double h;
	double h1 = sqrt(pow(((double)locationX - (double)x1), 2) + pow((double)locationY - (double)y1, 2));
	double h2 = sqrt(pow(((double)locationX - (double)x2), 2) + pow((double)locationY - (double)y2, 2));
	if (h1 < h2)
	{
		h = h1;
		target = new Cell(y1, x1, nullptr, 0, 0);
	}
	else
	{
		h = h2;
		target = new Cell(y2, x2, nullptr, 0, 0);
	}
	start = new Cell(locationY, locationX, nullptr, 0, h);
}

void NPC::SetSecurePoint(MazeHandler* maze, NPC opponent, Room room)
{
	double h = 200, temp;
	int r = 0, c = 0;
	vector<Cell*> nodes = room.GetObstacles();
	Cell* pCurrent;
	while (!nodes.empty())
	{
		pCurrent = nodes.back(); 
		nodes.pop_back(); 

		// opponent up
		if (opponent.getLocationY() > locationY)
		{
			temp = sqrt(pow(((double)locationX - (double)pCurrent->getCol()), 2) +
				pow((double)locationY - (double)pCurrent->getRow() - 1, 2));
			if (maze->maze[pCurrent->getRow() - 1][pCurrent->getCol()] == SPACE && temp < h)
			{
				c = pCurrent->getCol();
				r = pCurrent->getRow() - 1;
				h = temp;
			}
		}

		// opponent down
		if (opponent.getLocationY() < locationY)
		{
			temp = sqrt(pow(((double)locationX - (double)pCurrent->getCol()), 2) +
				pow((double)locationY - (double)pCurrent->getRow() + 1, 2));
			if (maze->maze[pCurrent->getRow() + 1][pCurrent->getCol()] == SPACE && temp < h)
			{
				c = pCurrent->getCol();
				r = pCurrent->getRow() + 1;
				h = temp;
			}
		}

		// opponent left
		if (opponent.getLocationX() < locationX)
		{
			temp = sqrt(pow(((double)locationX - (double)pCurrent->getCol()) + 1, 2) +
				pow((double)locationY - (double)pCurrent->getRow(), 2));
			if (maze->maze[pCurrent->getRow()][pCurrent->getCol() + 1] == SPACE && temp < h)
			{
				c = pCurrent->getCol() + 1;
				r = pCurrent->getRow();
				h = temp;
			}
		}

		// opponent right
		if (opponent.getLocationX() > locationX)
		{
			temp = sqrt(pow(((double)locationX - (double)pCurrent->getCol() - 1), 2) +
				pow((double)locationY - (double)pCurrent->getRow(), 2));
			if (maze->maze[pCurrent->getRow()][pCurrent->getCol() - 1] == SPACE && temp < h)
			{
				c = pCurrent->getCol() - 1;
				r = pCurrent->getRow();
				h = temp;
			}
		}

	}

	maze->maze[r][c] = SECURE_POINT;
	target = new Cell(r, c, nullptr, 0, 0);
	start = new Cell(locationY, locationX, nullptr, 0, h);
}

void NPC::SetPath(Cell* current_node)
{
	while (!current_node->operator==(*start))
	{
		path.push_back(current_node);
		current_node = current_node->getParent();
	}
}

void NPC::EmptyPath()
{
	while (!path.empty())
		path.pop_back();
}

void NPC::Move(MazeHandler* maze, int type_of_points)
{
	int minX = 0, minY = 0, maxX = 0, maxY = 0;
	if (room_index != PATH_INDEX)
	{
		minX = maze->rooms[room_index].getCenterX() - maze->rooms[room_index].getWidth() / 2;
		maxX = minX + maze->rooms[room_index].getWidth();
		minY = maze->rooms[room_index].getCenterY() - maze->rooms[room_index].getHeight() / 2;
		maxY = minY + maze->rooms[room_index].getHeight();
	}

	if (!path.empty()) 
		WalkInPath(maze, minX, minY, maxX, maxY);

	else 
		TargetAchieved(type_of_points);
}

void NPC::WalkInPath(MazeHandler* maze, int minX, int minY, int maxX, int maxY)
{
	maze->maze[locationY][locationX] = SPACE; 
	Cell* pcurrent = path.back(); 
	path.pop_back(); 

	if (maze->maze[pcurrent->getRow()][pcurrent->getCol()] == SPACE && !isInRoom) 
	{
		room_index = CalculateRoomIndex(maze);
		if (room_index != PATH_INDEX)
			isInRoom = true;
	}
	else if (room_index == PATH_INDEX) 
		isInRoom = false;
	else if (pcurrent->getRow() < minY || pcurrent->getRow() > maxY ||
		pcurrent->getCol() < minX || pcurrent->getCol() > maxX)
		isInRoom = false;

	locationX = pcurrent->getCol();
	locationY = pcurrent->getRow();
	maze->maze[locationY][locationX] = color;
}

void NPC::TargetAchieved(int type_of_points)
{
	if (type_of_points == HEALT_POINTS)
	{
		health += HEALTH_POINTS_STOCK;
		if (color == TEAM_1)
			cout << "Soldier from team 1 took Health Points!, Health: " << health << "\n";
		else
			cout << "Soldier from team 2 took Health Points!, Health: " << health << "\n";
	}
	else if (type_of_points == AMMO)
	{
		bullets_ammo += BULLETS_STOCK;
		grenade_ammo += GRENADE_STOCK;
		if (color == TEAM_1)
			cout << "Soldier from team 1 took Ammo!, Bullets:" << bullets_ammo << " Grenades: " << grenade_ammo << "\n";
		else
			cout << "Soldier from team 2 took Ammo!, Bullets:" << bullets_ammo << " Grenades: " << grenade_ammo << "\n";
	}

	target = nullptr; 
}

int NPC::CalculateRoomIndex(MazeHandler* maze)
{
	int i, minX, minY, maxX, maxY;
	for (i = 0; i < NUM_ROOMS; i++)
	{
		minX = maze->rooms[i].getCenterX() - maze->rooms[i].getWidth() / 2;
		maxX = minX + maze->rooms[i].getWidth();
		minY = maze->rooms[i].getCenterY() - maze->rooms[i].getHeight() / 2;
		maxY = minY + maze->rooms[i].getHeight();
		if (locationY >= minY && locationY <= maxY && locationX >= minX && locationX <= maxX) 
		{
			return i;
		}
	}

	return PATH_INDEX;
}

void NPC::Shoot(NPC opponent_team[NUM_SOLDIERS], MazeHandler* maze)
{
	if (this->getType() == SOLDIER)
	{
		if (health <= 0)
			return;

		if (pb == nullptr) 
			SetBulletDetails(opponent_team);

		else if (pb->getIsMoving())
			ShotMovement(maze, opponent_team);

		else 
			pb = nullptr;
	}
}

void NPC::SetBulletDetails(NPC opponent_team[NUM_SOLDIERS])
{
	double bx, by, dirx = 0, diry = 0, xdistance, ydistance, size;
	for (int i = 0; i < NUM_SOLDIERS; i++)
	{
		if (room_index == opponent_team[i].getRoomIndex())
		{
			if (bullets_ammo > 0)
			{
				bx = 2 * (double)locationX / (double)MSZ - 1;
				by = 2 * (double)locationY / (double)MSZ - 1;

				xdistance = (double)opponent_team[i].getLocationX() - (double)locationX;
				ydistance = (double)opponent_team[i].getLocationY() - (double)locationY;
				size = sqrt(pow(xdistance, 2) + pow(ydistance, 2));
				dirx = xdistance / size;
				diry = ydistance / size;
				pb = new Bullet(bx, by, false, dirx, diry); 
				pb->Shoot(); 
				bullets_ammo--;
				break;
			}
		}
	}
}

void NPC::ShotMovement(MazeHandler* maze, NPC opponent_team[NUM_SOLDIERS])
{
	double distance;
	int damage, row, col, i;
	i = 0;
	NPC* pOpponent;
	while (i < BULLET_MOVEMENT && pb->getIsMoving()) 
	{
		row = int(MSZ * (pb->getY() + 1) / 2);
		col = int(MSZ * (pb->getX() + 1) / 2);

		if ((maze->maze[row][col] == TEAM_1 && color != TEAM_1) ||
			(maze->maze[row][col] == TEAM_2 && color != TEAM_2))
		{
			for (int j = 0; j < NUM_SOLDIERS; j++) 
			{
				pOpponent = &opponent_team[j];
				if (row == pOpponent->getLocationY() && col == pOpponent->getLocationX())
				{
					distance = sqrt(pow(((double)locationX - pOpponent->getLocationX()), 2) +
						pow((double)locationY - pOpponent->getLocationY(), 2));
					damage = int(2.0 * 100 / distance); 
					pOpponent->UpdateHealth(-damage);

					if (pOpponent->getHealth() < 0)
						pOpponent->setHealth(0);

					if (pOpponent->getColor() == TEAM_1)
						cout << troopTypes[pOpponent->getType()] << " from team 1 Was Wounded!, Health: " <<  pOpponent->getHealth() << "\n";
					else
						cout << troopTypes[pOpponent->getType()] << " from team 2 Was Wounded!, Health: " << pOpponent->getHealth() << "\n";

					pb->stopMoving();

					if (pOpponent->getHealth() <= 0) 
					{
						if (pOpponent->getColor() == TEAM_1)
							cout << troopTypes[pOpponent->getType()] << " from team 1 Died!\n";
						else
							cout << troopTypes[pOpponent->getType()] << " from team 2 Died!\n";
					}
					break;
				}
			}
		}
		pb->Move(maze->maze);
		i++;
	}
	pb->Show();
}

void NPC::ThrowGrenade(NPC opponent_team[NUM_SOLDIERS], MazeHandler* maze)
{
	if (this->getType() == SOLDIER)
	{
		if (health <= 0)
			return;

		if (pg == nullptr) 
			SetGrenadeDetails(opponent_team, maze);

		else if (pg->getIsExploded())
			GrenadeMovement(maze, opponent_team);

		else
			pg = nullptr;
	}
}

void NPC::SetGrenadeDetails(NPC opponent_team[NUM_SOLDIERS], MazeHandler* maze)
{
	NPC* pOpponent;
	double bx, by, dirx = 0, diry = 0, xdistance, ydistance, size;
	for (int i = 0; i < NUM_SOLDIERS; i++)
	{
		if (room_index == opponent_team[i].getRoomIndex())
		{
			pOpponent = &opponent_team[i];
			if (grenade_ammo > 0)
			{
				bx = 2 * (double)locationX / (double)MSZ - 1;
				by = 2 * (double)locationY / (double)MSZ - 1;

				pg = new Grenade(bx, by); 

				xdistance = (double)pOpponent->getLocationX() - (double)locationX;
				ydistance = (double)pOpponent->getLocationY() - (double)locationY;
				size = sqrt(pow(xdistance, 2) + pow(ydistance, 2));
				dirx = xdistance / size;
				diry = ydistance / size;

				pg->setDirX(dirx);
				pg->setDirY(diry);

				pg->Throw(maze->maze, locationX, locationY, pOpponent->getLocationX(), pOpponent->getLocationY());
				grenade_ammo--;
				pg->Explode();
				break;
			}
		}
	}
}

void NPC::GrenadeMovement(MazeHandler* maze, NPC opponent_team[NUM_SOLDIERS])
{
	int row, col, i;
	bool isAlive = true;
	Bullet* pBullet;

	row = int(pg->getY());
	col = int(pg->getX());
	if (row != pg->getTargetY() || col != pg->getTargetX())
		pg->Throw(maze->maze, 0, 0, 0, 0); 

	else
	{
		i = 0;
		while (i < BULLET_MOVEMENT && pg->getIsExploded()) 
		{
			for (int j = 0; j < NUM_BULLETS; j++)
			{
				pBullet = pg->GetBulletFromIndex(j);
				row = int(MSZ * (pBullet->getY() + 1) / 2);
				col = int(MSZ * (pBullet->getX() + 1) / 2);
				if ((maze->maze[row][col] == TEAM_1 && color != TEAM_1) ||
					(maze->maze[row][col] == TEAM_2 && color != TEAM_2))
				{
					GrenadeHit(opponent_team, row, col, pBullet);
				}
			}
			i++;
			pg->Exploding(maze->maze);
		}
	}
	pg->Show();
}

void NPC::GrenadeHit(NPC opponent_team[NUM_SOLDIERS], int row, int col, Bullet* pBullet)
{
	int damage;
	double distance;
	bool isAlive = true;
	NPC* pOpponent;

	for (int k = 0; k < NUM_SOLDIERS; k++)
	{
		pOpponent = &opponent_team[k];
		if (row == pOpponent->getLocationY() && col == pOpponent->getLocationX() && pBullet->getIsMoving())
		{
			distance = sqrt(pow(((double)pg->getX() - pOpponent->getLocationX()), 2) +
				pow((double)pg->getY() - pOpponent->getLocationY(), 2));
			damage = int(100 / distance);
			pOpponent->UpdateHealth(-damage);
			if (pOpponent->getHealth() < 0)
			{
				pOpponent->setHealth(0);
				if (pOpponent->getColor() == TEAM_1 && isAlive)
					cout << troopTypes[pOpponent->getType()] << "from team 1 Died By Grenade! " << pOpponent->getHealth() << "\n";
				else if (isAlive)
					cout << troopTypes[pOpponent->getType()] << " from team 2 Died By Grenade! " << pOpponent->getHealth() << "\n";
				isAlive = false;
			}
			else
			{
				if (pOpponent->getColor() == TEAM_1)
					cout << troopTypes[pOpponent->getType()] << " from team 1 Was Wounded By Grenade!, Health: " << pOpponent->getHealth() << "\n";
				else
					cout << troopTypes[pOpponent->getType()] << " from team 2 Was Wounded By Grenade!, Health: " << pOpponent->getHealth() << "\n";
			}
			pBullet->stopMoving();
			break;
		}
	}
}

/* Adding neighbors to calculate the next step of A* */
void NPC::AddNextStep(Cell* pCurrent, Direction directionType, priority_queue <Cell*, vector<Cell*>, CompareCells>& pq,
	vector <Cell>& gray, vector <Cell>& black, MazeHandler* maze)
{
	int i = 0, j = 0;
	Cell* pn;
	double g, h;
	vector<Cell>::iterator it_gray;
	vector<Cell>::iterator it_black;

	switch (directionType)
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
	h = maze->Distance(j, i, target->getCol(), target->getRow());
	g = pCurrent->getG() + maze->security_map[i][j];

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
				maze->UpdatePQ(pq, pn);
				gray.erase(it_gray);
				gray.push_back(*pn);
			}
		}
	}
}

/* Find the shortest path to target using A* algorithm */
void NPC::FindPath(MazeHandler* maze)
{
	vector <Cell> gray;
	vector <Cell> black;
	priority_queue<Cell*, vector<Cell*>, CompareCells> pq;
	vector<Cell>::iterator it_gray;
	vector<Cell>::iterator it_black;
	Cell* pn;
	Cell* pCurrent;
	int startY, startX, targetY, targetX;
	startY = locationY;
	startX = locationX;
	targetY = target->getRow();
	targetX = target->getCol();
	pn = new Cell(startY, startX, nullptr, 0, maze->Distance(startX, startY, targetX, targetY));
	pq.push(pn);
	gray.push_back(*pn);
	while (!pq.empty())
	{
		pCurrent = pq.top();
		pq.pop(); 
		if (pCurrent->operator==(*target))
		{
			SetPath(pCurrent);
			return; 
		}
		else 
		{
			black.push_back(*pCurrent);
			it_gray = find(gray.begin(), gray.end(), *pCurrent); 
			if (it_gray != gray.end()) 
				gray.erase(it_gray);

				// check up neighbor
			if (maze->maze[pCurrent->getRow() + 1][pCurrent->getCol()] == SPACE ||
				maze->maze[pCurrent->getRow() + 1][pCurrent->getCol()] ==
				maze->maze[target->getRow()][target->getCol()])
				AddNextStep(pCurrent, TOP, pq, gray, black, maze);

			// check down neighbor
			if (maze->maze[pCurrent->getRow() - 1][pCurrent->getCol()] == SPACE ||
				maze->maze[pCurrent->getRow() - 1][pCurrent->getCol()] ==
				maze->maze[target->getRow()][target->getCol()])
				AddNextStep(pCurrent, BOTTOM, pq, gray, black, maze);

			// check left neighbor
			if (maze->maze[pCurrent->getRow()][pCurrent->getCol() - 1] == SPACE ||
				maze->maze[pCurrent->getRow()][pCurrent->getCol() - 1] ==
				maze->maze[target->getRow()][target->getCol()])
				AddNextStep(pCurrent, LEFT, pq, gray, black, maze);

			// check right neighbor
			if (maze->maze[pCurrent->getRow()][pCurrent->getCol() + 1] == SPACE ||
				maze->maze[pCurrent->getRow()][pCurrent->getCol() + 1] ==
				maze->maze[target->getRow()][target->getCol()])
				AddNextStep(pCurrent, RIGHT, pq, gray, black, maze);
		}
	}
}

void NPC::SearchForAmmo(MazeHandler* maze, Ammo* ammo1, Ammo* ammo2, NPC* pS)
{
	if(isAttacking)
		pCurrentState->Transform(this);

	if (target == nullptr)
	{
		FindTargetPoint(ammo1->getX(), ammo1->getY(), ammo2->getX(), ammo2->getY());
		FindPath(maze);
	}
	else if (maze->maze[target->getRow()][target->getCol()] != AMMO)
	{
		if (ammo1->getX() != RANDOM_VALUE || pS->getLocationX() != RANDOM_VALUE)
		{
			FindTargetPoint(ammo1->getX(), ammo1->getY(), pS->getLocationX(), pS->getLocationY());
			EmptyPath(); 
			FindPath(maze);
		}
	}

	if (locationX == target->getCol() && locationY == target->getRow())
		maze->RemoveAmmoPoint(ammo1, ammo2, locationY, locationX);

	Move(maze, AMMO); 
}

void NPC::SearchForHealth(MazeHandler* maze, HealthPoints* hp1, HealthPoints* hp2, NPC* pS)
{
	if(isAttacking)
		pCurrentState->Transform(this);

	if (target == nullptr)
	{
		FindTargetPoint(hp1->getX(), hp1->getY(), pS->getLocationX(), pS->getLocationY());
		FindPath(maze);
	}
	else if (maze->maze[target->getRow()][target->getCol()] != HEALT_POINTS)
	{
		if (hp1->getX() != RANDOM_VALUE || hp2->getX() != RANDOM_VALUE)
		{
			FindTargetPoint(hp1->getX(), hp1->getY(), pS->getLocationX(), pS->getLocationY());
			EmptyPath(); 
			FindPath(maze);
		}
	}

	if (locationX == target->getCol() && locationY == target->getRow())
		maze->RemoveHealthPoint(hp1, hp2, locationY, locationX);

	Move(maze, HEALT_POINTS); 	
}

void NPC::AttackEnemy(MazeHandler* maze, NPC opponent_team[NUM_SOLDIERS], NPC opponent)
{
	if (isSearchingAmmoAndHealth || isSearchingCover || isSearchingEnemy)
		pCurrentState->Transform(this);

	if ((CheckIfSecure(maze, opponent) || opponent.CheckIfSecure(maze, *this)) && grenade_ammo > 0)
		ThrowGrenade(opponent_team, maze);

	else if (bullets_ammo > 0)
		Shoot(opponent_team, maze);
}

void NPC::SearchForCover(MazeHandler* maze, NPC opponent)
{
	if (isAttacking)
		pCurrentState->Transform(this);

	double x, y;
	x = 2 * (double)opponent.getLocationX() / (double)MSZ - 1;
	y = 2 * (double)opponent.getLocationY() / (double)MSZ - 1;
	Grenade* pg = new Grenade(x, y);
	pg->SimulateSecurityMap(maze->maze, maze->security_map);
	if (target == nullptr)
	{
		SetSecurePoint(maze, opponent, maze->rooms[room_index]);
		FindPath(maze);
	}
	else if (maze->maze[target->getRow()][target->getCol()] != SECURE_POINT)
	{
		SetSecurePoint(maze, opponent, maze->rooms[room_index]);
		EmptyPath(); 
		FindPath(maze);
	}

	if (locationX == target->getCol() && locationY == target->getRow())
		maze->maze[locationY][locationX] = SPACE;

	Move(maze, SECURE_POINT); 
}

void NPC::MoveToRandomRoom(MazeHandler* maze) //Run away
{
	if (isSearchingCover)
		pCurrentState->Transform(this);

	int room_index, minX = 0, minY = 0, x = 0, y = 0;
	double h = 0;
	Cell* pn = nullptr;
	if (target == nullptr)
	{
		room_index = rand() % NUM_ROOMS;
		if (maze->maze[maze->rooms[room_index].getCenterY()][maze->rooms[room_index].getCenterX()] == SPACE)
		{
			pn = new Cell(maze->rooms[room_index].getCenterY(), maze->rooms[room_index].getCenterX(), nullptr, 0, 0);
			h = maze->Distance(locationX, locationY, maze->rooms[room_index].getCenterX(), maze->rooms[room_index].getCenterY());
		}
		else 
		{
			minX = maze->rooms[room_index].getCenterX() - maze->rooms[room_index].getWidth() / 2;
			minY = maze->rooms[room_index].getCenterY() - maze->rooms[room_index].getHeight() / 2;

			while (maze->maze[y][x] != SPACE)
			{
				x = rand() % maze->rooms[room_index].getWidth() + minX;
				y = rand() % maze->rooms[room_index].getHeight() + minY;
			}
			h = maze->Distance(locationX, locationY, x, y);
			pn = new Cell(y, x, nullptr, 0, 0);
		}
		target = pn;
		start = new Cell(locationY, locationX, nullptr, 0, h);
		FindPath(maze);
	}

	Move(maze, MOVEMENT);
}

void NPC::SearchForEnemy(MazeHandler* maze, NPC opponent_team[NUM_SOLDIERS], bool* withEnemy)
{
	if (isSearchingAmmoAndHealth || isSearchingAmmoAndHealth)
		pCurrentState->Transform(this);

	int i;
	for (i = 0; i < NUM_SOLDIERS; i++)
	{
		if (room_index == opponent_team[i].getRoomIndex() && room_index != PATH_INDEX)
		{
			AttackEnemy(maze, opponent_team, opponent_team[i]);
			SearchForCover(maze, opponent_team[i]);
			*withEnemy = true;
		}
	}
}
