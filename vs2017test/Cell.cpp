#include "Cell.h"
#include <math.h>


Cell::Cell()
{
	row = 0;
	col = 0;
	parent = nullptr;
	g = 0;
	h = 0;
	f = g + h;
}

Cell::Cell(int r, int c, Cell *p, double g, double h)
{	
	row = r;
	col = c;
	parent = p;
	this->g = g;
	this->h = h;
	f = g + h;
}


Cell::~Cell()
{
}
