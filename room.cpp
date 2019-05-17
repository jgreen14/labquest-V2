#include "room.h"

/*** ROOM CLASS ***/
Room::Room(int y, int x, int width, int height)
{
    this->y = y;
    this->x = x;
    this->width = width;
    this->height = height;
}//Room

Room::Room()
{
    this->y = 0;
    this->x = 0;
    this->width = 0;
    this->height = 0;
}//Room

void Room::setY(int y)
{
    this->y = y;
}//setY

int Room::getY()
{
    return this->y;
}//getY

void Room::setX(int x)
{
    this->x = x;
}//setX

int Room::getX()
{
    return this->x;
}//getX

void Room::setWidth(int width)
{
    this->width = width;
}//setWidth

int Room::getWidth()
{
    return this->width;
}//getWidth

void Room::setHeight(int height)
{
    this->height = height;
}//setHeight

int Room::getHeight()
{
    return this->height;
}//getHeight

/*** END ROOM CLASS ***/
