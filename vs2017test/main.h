#pragma once
#include "glut.h"
#include <time.h>
#include <vector>
#include <iostream>
#include <windows.h>
#include "Cell.h"
#include "Room.h"
#include <queue>
#include "CompareCells.h"
#include "Bullet.h"
#include <math.h>
#include "Grenade.h"
#include "Ammo.h"
#include "HealthPoints.h"
#include "NPC.h"
#include "Teams.h"
#include "MazeHandler.h"
#include "Definitions.h"

void init();
void CreateTeams();
void GenerateTeamPosition(NPC team[NUM_SOLDIERS], int roomNumber, int team_color);
void RemoveDeadSoldiers();
void SwitchTurn(NPC* pS);
void CheckGameOver(bool cyan_alive, bool olive_alive);
void Play();
void display();
void idle();
void menu(int choice);
void mouse(int button, int state, int x, int y);