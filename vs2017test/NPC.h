#pragma once
#include "Ammo.h"
#include "HealthPoints.h"
#include "Grenade.h"
#include "Bullet.h"
#include "Cell.h"
#include "CompareCells.h"
#include <vector>
#include <queue>
#include "MazeHandler.h"
#include "Definitions.h"

class State;
using namespace std;

const enum eType { SOLDIER, CARRIER };

class NPC
{
private:
	bool isInRoom;
	int locationX;
	int locationY;
	int health;
	int bullets_ammo;
	int grenade_ammo;
	int health_stock;
	int bullets_stock;
	int grenade_stock;
	int room_index;
	int color;
	eType type;
	State* pCurrentState;

	bool isSearchingEnemy, isSearchingAmmoAndHealth, isSearchingCover, isAttacking, isSuplying;

	Cell *start, *target;
	vector<Cell*> path;

	Bullet* pb;
	Grenade* pg;

public:
	NPC();
	NPC(int x, int y, int room, int team_color, eType type);

	int getLocationX() { return locationX; }
	int getLocationY() { return locationY; }
	int getHealth() { return health; }
	State* getCurrentState() { return pCurrentState; }
	bool getIsSearchingEnemy() { return isSearchingEnemy; }
	bool getIsSearchingAmmoAndHealth() { return isSearchingAmmoAndHealth; }
	bool getIsSearchingCover() { return isSearchingCover; }
	bool getIsAttacking() { return isAttacking; }
	bool getIsSuplying() { return isSuplying; }
	int getColor() { return color; }
	int getRoomIndex() { return room_index; }
	eType getType() { return type; }

	/*Soldier*/
	int getBulletsAmmo() { return bullets_ammo; } 
	int getGrenadeAmmo() { return grenade_ammo; } 
	/*Carrier*/
	int getHealthStock() { return health; }
	int getBulletsStock() { return bullets_stock; }
	int getGrenadeStock() { return grenade_stock; }
	
	void setColor(int team_color) { color = team_color; }
	void setRoomIndex(int index) { room_index = index; }
	void setLocationX(int lx) { locationX = lx; }
	void setLocationY(int ly) { locationY = ly; }
	void setType(int t) { type = (eType)t; }
	void setHealth(int he) { health = he; }
	void setCurrentState(State* ps) { pCurrentState = ps; }
	void setIsSearchingEnemy(bool b) { isSearchingEnemy = b; }
	void setIsSearchingAmmoAndHealth(bool b) { isSearchingAmmoAndHealth = b; }
	void setIsSearchingCover(bool b) { isSearchingCover = b; }
	void setIsAttacking(bool b) { isAttacking = b; }
	void setIsSuplying(bool b) { isSuplying = b; }
	void SetSecurePoint(MazeHandler* maze, NPC opponent, Room room);

	/*Soldier*/
	void UpdateHealth(int health_points) { health += health_points; }
	void UpdateBulletsAmmo(int num_of_bullets) { bullets_ammo += num_of_bullets; }
	void UpdateGrenadeAmmo(int num_of_grenades) { grenade_ammo += num_of_grenades; }
	/*Carrier*/
	void UpdateHealthStock(int health_points) { health += health_points; }
	void UpdateBulletsStock(int num_of_bullets) { bullets_ammo += num_of_bullets; }
	void UpdateGrenadeStock(int num_of_grenades) { grenade_ammo += num_of_grenades; }

	bool CheckIfSecure(MazeHandler* maze, NPC opponent);
	void FindTargetPoint(int x1, int y1, int x2, int y2);
	void FindPath(MazeHandler* maze);
	void SetPath(Cell* current_node);
	void EmptyPath();
	void WalkInPath(MazeHandler* maze, int minX, int minY, int maxX, int maxY);
	void TargetAchieved(int type_of_points);
	
	void Move(MazeHandler* maze, int type_of_points);
	void MoveToRandomRoom(MazeHandler* maze);
	int  CalculateRoomIndex(MazeHandler* maze);
	/*--------Only Soldier--------*/
	void Shoot(NPC opponent_team[NUM_SOLDIERS], MazeHandler* maze);
	void ThrowGrenade(NPC opponent_team[NUM_SOLDIERS], MazeHandler* maze);
	void AttackEnemy(MazeHandler* maze, NPC opponent_team[NUM_SOLDIERS], NPC opponent);
	/*Search*/
	void SearchForEnemy(MazeHandler* maze, NPC opponent_team[NUM_SOLDIERS], bool* withEnemy);
	void SearchForAmmo(MazeHandler* maze, Ammo* ammo1, Ammo* ammo2, NPC* pS);
	void SearchForHealth(MazeHandler* maze, HealthPoints* hp1, HealthPoints* hp2, NPC* pS);
	void SearchForCover(MazeHandler* maze, NPC opponent);
	/*Bullet*/
	void SetBulletDetails(NPC opponent_team[NUM_SOLDIERS]);
	void ShotMovement(MazeHandler* maze, NPC opponent_team[NUM_SOLDIERS]);
	/*Grenade*/
	void SetGrenadeDetails(NPC opponent_team[NUM_SOLDIERS], MazeHandler* maze);
	void GrenadeMovement(MazeHandler* maze, NPC opponent_team[NUM_SOLDIERS]);
	void GrenadeHit(NPC opponent_team[NUM_SOLDIERS], int row, int col, Bullet* pBullet);
	
	void AddNextStep(Cell* pCurrent, Direction directionType, priority_queue <Cell*, vector<Cell*>, CompareCells>& pq,
		vector <Cell>& gray, vector <Cell>& black, MazeHandler* maze);
};


