#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "NPC.h"
#include "glut.h"
#include <stdio.h>
#include "Definitions.h"

class Teams
{
private:
	double x, y;
	NPC* team1;
	NPC* team2;
	char text[Size];
	char t[Size];

public:
	Teams(double x, double y, NPC t1[NUM_SOLDIERS], NPC t2[NUM_SOLDIERS]);
	void Draw();
	void drawBitmapText(const char* string, float x, float y, float z);
};

