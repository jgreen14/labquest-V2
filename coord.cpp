#include "coord.h"

/*** COORD CLASS ***/
Coord::Coord(int y, int x)
{
	this->y = y;
	this->x = x;
}//Coord

Coord::Coord()
{
	this->y = 0;
	this->x = 0;
}//Coord

/*** GETTERS/SETTERS ***/
int Coord::getY()
{
	return this->y;
}//getY

int Coord::getX()
{
	return this->x;
}//getY

void Coord::setY(int y)
{
	this->y = y;
}//setY

void Coord::setX(int x)
{
	this->x = x;
}//setY

void Coord::setYX(int y, int x)
{
	this->y = y;
	this->x = x;
}//setY

bool Coord::equal(int y, int x)
{
    if (this->y == y && this->x == x) 
        return true;
    else
        return false;
}//equal

bool Coord::equal(Coord* coord)
{
    if (this->y == coord->getY() && this->x == coord->getX())
        return true;
    else
        return false;
}//equal
/*** END COORD CLASS ***/
