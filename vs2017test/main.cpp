#include "main.h"

using namespace std;

Ammo* ammo1, *ammo2;
HealthPoints* hp1, *hp2;
NPC team1[NUM_SOLDIERS];
NPC team2[NUM_SOLDIERS];

Teams* pd = nullptr;
MazeHandler* maze = nullptr;

bool isPlaying = false;

void init()
{
	maze = new MazeHandler();
	ammo1 = new Ammo();
	ammo2 = new Ammo();
	maze->AddAmmoStock(ammo1, ammo2);
	hp1 = new HealthPoints();
	hp2 = new HealthPoints();
	maze->AddHealthPoints(hp1, hp2);
	CreateTeams();
}

void CreateTeams()
{
	int team1_room_index, team2_room_index;
	team1_room_index = rand() % NUM_ROOMS;
	GenerateTeamPosition(team1, team1_room_index, TEAM_1);

	do
	{
		team2_room_index = rand() % NUM_ROOMS;
	} while (team2_room_index == team1_room_index);

	GenerateTeamPosition(team2, team2_room_index, TEAM_2);
}

void GenerateTeamPosition(NPC team[NUM_SOLDIERS], int roomNumber, int team_color)
{
	NPC* pS;
	int randomX, randomY;
	int soldier = 0, carrier = 1;
	for (int i = 0; i < NUM_SOLDIERS; i++)
	{
		pS = &team[i];

		do
		{
			randomX = rand() % maze->rooms[roomNumber].getWidth() +
				(maze->rooms[roomNumber].getCenterX() - maze->rooms[roomNumber].getWidth() / 2);
			randomY = rand() % maze->rooms[roomNumber].getHeight() +
				(maze->rooms[roomNumber].getCenterY() - maze->rooms[roomNumber].getHeight() / 2);
		
			pS->setLocationX(randomX);
			pS->setLocationY(randomY);
		} while (maze->maze[pS->getLocationY()][pS->getLocationX()] != SPACE);
		
		if (i == 0)
			pS->setType(carrier);
		else
			pS->setType(soldier);
		
		pS->setColor(team_color);
		pS->setRoomIndex(roomNumber);
		maze->maze[pS->getLocationY()][pS->getLocationX()] = pS->getColor();
	}
}

void RemoveDeadSoldiers()
{
	int i;
	for (i = 0; i < NUM_SOLDIERS; i++)
	{
		if (team1[i].getHealth() <= 0)
		{
			maze->maze[team1[i].getLocationY()][team1[i].getLocationX()] = SPACE;
			team1[i].setRoomIndex(PATH_INDEX);
		}

		if (team2[i].getHealth() <= 0)
		{
			maze->maze[team2[i].getLocationY()][team2[i].getLocationX()] = SPACE;
			team2[i].setRoomIndex(PATH_INDEX);
		}
	}
}

void SwitchTurn(NPC* pS)
{
	bool withEnemy = false;

	if (pS->getHealth() <= 0)
		return;
	
	if ((pS->getBulletsAmmo() < BULLETS_STOCK || pS->getGrenadeAmmo() < GRENADE_STOCK)
		&& (ammo1->getX() != RANDOM_VALUE || ammo2->getX() != RANDOM_VALUE))
	{
		if (pS->getColor() == TEAM_1 && pS->getType() != 1)
			pS->SearchForAmmo(maze, ammo1, ammo2, &team1[0]);
		else
			pS->SearchForAmmo(maze, ammo1, ammo2, &team2[0]);
	}

	else if (pS->getHealth() < HEALTH_POINTS_STOCK && (hp1->getX() != RANDOM_VALUE || hp2->getX() != RANDOM_VALUE))
	{
		if (pS->getColor() == TEAM_1 && pS->getType() != 1)
			pS->SearchForHealth(maze, hp1, hp2, &team1[0]);
		else
			pS->SearchForHealth(maze, hp1, hp2, &team2[0]);
	}
	
	else
	{
		if (pS->getColor() == TEAM_1)
			pS->SearchForEnemy(maze, team2, &withEnemy);
		else
			pS->SearchForEnemy(maze, team1, &withEnemy);

		if (!withEnemy)
			pS->MoveToRandomRoom(maze);
	}
}

void CheckGameOver(bool team_num_1, bool team_num_2)
{
	if (!team_num_1 && team_num_2)
	{
		cout << "Team 1 Won!\n";
		isPlaying = false;
	}
	else if (team_num_1 && !team_num_2)
	{
		cout << "Team 2 Won!\n";
		isPlaying = false;
	}
	else if (!team_num_1 && !team_num_2)
	{
		cout << "It's a Tie!";
		isPlaying = false;
	}
}

void Play()
{
	Sleep(200);
	int i;
	bool team1_alive = false, team2_alive = false;
	for (i = 0; i < NUM_SOLDIERS; i++)
	{
		SwitchTurn(&team1[i]);
		SwitchTurn(&team2[i]);
	}

	RemoveDeadSoldiers();

	if (isPlaying)
	{
		for (i = 0; i < NUM_SOLDIERS; i++)
		{
			if (team1[i].getHealth() > 0)
				team1_alive = true;
			if (team2[i].getHealth() > 0)
				team2_alive = true;
		}
		CheckGameOver(team1_alive, team2_alive);
	}

	if (!isPlaying)
		glutPostRedisplay(); 
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	maze->ShowMaze();

	if (isPlaying)
		Play();

	if (pd != nullptr)
		pd->Draw();

	glutSwapBuffers();
}

void idle()
{
	if (pd != nullptr)
		pd->Draw();

	glutPostRedisplay(); 
}

void menu(int choice)
{
	if (choice == 1) 
		isPlaying = true;

	else if (choice == 2)
		maze->UpdateSecurityMap();
}

void mouse(int button, int state, int x, int y)
{
	double bx, by;

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{

		bx = (double)2 * x / (double)W - 1; 
		by = (double)2 * ((double)H - y) / (double)H - 1;

		pd = new Teams(bx, by, team1, team2);
	}
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(W, H);
	glutInitWindowPosition(700, 100);
	glutCreateWindow("Dungeon Project");
	glutDisplayFunc(display); 
	glutIdleFunc(idle); 
	glutMouseFunc(mouse);
	glutCreateMenu(menu);
	glutAddMenuEntry("Start Game", 1);
	glutAddMenuEntry("Create Security Map", 2);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	init();
	glutMainLoop();

	return 0;
}