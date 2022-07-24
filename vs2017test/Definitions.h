#pragma once
const int Size = 30;
const int MSZ = 100;
const int WALL = 1;
const int SPACE = 2;
const int START = 3;
const int PATH = 5;
const int GRAY = 6;
const int BLACK_SPACE = 7;
const int TARGET = 8;
const int AMMO = 9;
const int HEALT_POINTS = 10;
const int TEAM_1 = 11; 
const int TEAM_2 = 12; 
const int SECURE_POINT = 13;
const int MOVEMENT = 14;
const int NUM_ROOMS = 10;
/*Ammo*/
const int BULLETS_STOCK = 5;
const int GRENADE_STOCK = 2;
/*HP*/
const int HEALTH_POINTS_STOCK = 50;
/*Bullet*/
const double SPEED = 0.01; 
const int BULLET_MOVEMENT = 10;
/*Grenade*/
const int NUM_BULLETS = 15;
const double PI = 3.14;
const int GRENADE_MOVEMENT = 2;
/*main*/
const int W = 600; // window width
const int H = 600; // window height
/*mazeHandler*/
const int RANDOM_VALUE = 10000;
const double SPACE_COST = 0.05;
const double WALL_COST = 2;
/*NPC*/
const int START_BULLETS = 20;
const int START_GRENADES = 5;
const int START_HEALTH_POINTS = 100;
const int NUM_SOLDIERS = 3; // 2 Soldiers, 1 Carrier
const int PATH_INDEX = NUM_ROOMS + 10;
const int BASKET_BULLETS = 100;
const int BASKET_GRENADES = 15;
const int BASKET_HEALTH_POINTS = 200;