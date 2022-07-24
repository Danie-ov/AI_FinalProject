#pragma once

class Cell
{
private:
	int row, col;
	Cell* parent;
	double f, g, h;

public:
	Cell();
	Cell(int r, int c, Cell* p, double g, double h);
	~Cell();
	
	int getRow() { return row; }
	int getCol() { return col; }
	double getF() { return f; }
	double getG() { return g; }
	double getH() { return h; }
	Cell* getParent() { return parent; }
	
	void ComputeF() { f = g + h; }
	bool operator == (Cell other) { return row == other.row && col == other.col; }
	void setG(double newG) { g = newG; }
};

