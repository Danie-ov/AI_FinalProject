#include "Teams.h"

Teams::Teams(double x, double y, NPC t1[NUM_SOLDIERS], NPC t2[NUM_SOLDIERS]) : 
						x(x), y(y), team1(t1), team2(t2), text(""), t("")
{
}

void Teams::Draw()
{
	glColor3f(0.15, 0.15, 0.15);
		glBegin(GL_POLYGON);
		glVertex2d(x, y);
		glVertex2d(x + 0.50, y);
		glVertex2d(x + 0.50, y - 0.3);
		glVertex2d(x, y - 0.3);
	glEnd();

	if (team1[0].getHealth() == 0)
	{
		glColor3f(0.0, 1.0, 1.0);
		glLoadIdentity();
		strcpy(text, "First soldier from team 1: Dead");
		strcat(text, "\0");
		drawBitmapText(text, x + 0.02, y - 0.05, 0);
		glEnd();
	}
	else
	{
		glColor3f(0.0, 1.0, 1.0);
		glLoadIdentity();
		strcpy(text, "First soldier from team 1: H:");
		sprintf(t, " %d B: %d, G: %d, R: %d", team1[0].getHealth(), team1[0].getBulletsAmmo(), team1[0].getGrenadeAmmo(), team1[0].getRoomIndex());
		strcat(text, t);
		strcat(text, "\0");
		drawBitmapText(text, x + 0.02, y - 0.05, 0);
		glEnd();
	}


	if (team1[1].getHealth() == 0)
	{
		glColor3f(0.0, 1.0, 1.0);
		glLoadIdentity();
		strcpy(text, "Second soldier from team 1: Dead");
		strcat(text, "\0");
		drawBitmapText(text, x + 0.02, y - 0.12, 0);
		glEnd();
	}
	else
	{
		glColor3f(0.0, 1.0, 1.0);
		glLoadIdentity();
		strcpy(text, "Second soldier from team 1: H:");
		sprintf(t, " %d B: %d, G: %d, R: %d", team1[1].getHealth(), team1[1].getBulletsAmmo(), team1[1].getGrenadeAmmo(), team1[1].getRoomIndex());
		strcat(text, t);
		strcat(text, "\0");
		drawBitmapText(text, x + 0.02, y - 0.12, 0);
		glEnd();
	}

		if (team1[2].getHealth() == 0)
		{
			glColor3f(0.75, 0.1, 0.75);
			glLoadIdentity();
			strcpy(text, "Carrier from team 1: Dead");
			strcat(text, "\0");
			drawBitmapText(text, x + 0.02, y - 0.12, 0);
			glEnd();
		}
		else
		{
			glColor3f(0.75, 0.1, 0.75);
			glLoadIdentity();
			strcpy(text, "Carrier from team 1: H:");
			sprintf(t, " %d B: %d, G: %d, R: %d", team1[2].getHealthStock(), team1[2].getBulletsStock(), team1[2].getGrenadeStock(), team1[2].getRoomIndex());
			strcat(text, t);
			strcat(text, "\0");
			drawBitmapText(text, x + 0.02, y - 0.12, 0);
			glEnd();
		}


	if (team2[0].getHealth() == 0)
	{
		glColor3f(0.5, 0.5, 0.0);
		glLoadIdentity();
		strcpy(text, "First soldier from team 2: Dead");
		strcat(text, "\0");
		drawBitmapText(text, x + 0.02, y - 0.19, 0);
		glEnd();
	}
	else
	{
		glColor3f(0.5, 0.5, 0.0);
		glLoadIdentity();
		strcpy(text, "First soldier from team 2: H:");
		sprintf(t, " %d B: %d, G: %d, R: %d", team2[0].getHealth(), team2[0].getBulletsAmmo(), team2[0].getGrenadeAmmo(), team2[0].getRoomIndex());
		strcat(text, t);
		strcat(text, "\0");
		drawBitmapText(text, x + 0.02, y - 0.19, 0);
		glEnd();
	}

	if (team2[1].getHealth() == 0)
	{
		glColor3f(0.5, 0.5, 0.0);
		glLoadIdentity();
		strcpy(text, "Second soldier from team 2: Dead");
		strcat(text, "\0");
		drawBitmapText(text, x + 0.02, y - 0.26, 0);
		glEnd();
	}
	else
	{
		glColor3f(0.5, 0.5, 0.0);
		glLoadIdentity();
		strcpy(text, "Second soldier from team 2: H:");
		sprintf(t, " %d B: %d, G: %d, R: %d", team2[1].getHealth(), team2[1].getBulletsAmmo(), team2[1].getGrenadeAmmo(), team2[1].getRoomIndex());
		strcat(text, t);
		strcat(text, "\0");
		drawBitmapText(text, x + 0.02, y - 0.26, 0);
		glEnd();
	}

	if (team2[2].getHealthStock() == 0)
	{
		glColor3f(1.0, 0.4, .0);
		glLoadIdentity();
		strcpy(text, "Carrier from team 2: Dead");
		strcat(text, "\0");
		drawBitmapText(text, x + 0.02, y - 0.12, 0);
		glEnd();
	}
	else
	{
		glColor3f(1.0, 0.4, .0);
		glLoadIdentity();
		strcpy(text, "Carrier from team 2: Health point are empty!\n");
		sprintf(t, " %d B: %d, G: %d, R: %d", team2[2].getHealthStock(), team2[2].getBulletsStock(), team2[2].getGrenadeStock(), team2[2].getRoomIndex());
		strcat(text, t);
		strcat(text, "\0");
		drawBitmapText(text, x + 0.02, y - 0.12, 0);
		glEnd();
	}

}

void Teams::drawBitmapText(const char* string, float x, float y, float z)
{
	const char* c;
	glRasterPos3f(x, y, z);

	for (c = string; *c != '\0'; c++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, *c);
	}
}