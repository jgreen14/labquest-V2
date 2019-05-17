#include "creature.h"
#include "coord.h"
#include "items.h"
#include "level.h"

/*** CREATURE CLASS***/
Creature::Creature() : Agent()
{
	hostile = true;
	direction = STOPPED;
	type = UNKNOWN;
}//Creature

Creature::Creature(int str, int dex, int per, string name, Coord* position, char symbol) : Agent(str, dex, per, name, position, symbol)
{
	hostile = true;
	direction = STOPPED;
	type = UNKNOWN;
}//Creature

Creature::Creature(int str, int dex, int per, string name, Coord* position, char symbol, CreatureType type) : Agent(str, dex, per, name, position, symbol)
{
	hostile = true;
	direction = STOPPED;
	this->type = type;
}//Creature

bool Creature::move(Level* level)
{
	bool moved = false;
	int x = 0, y = 0;
	MouseTrap* trap;

	if (direction == STOPPED || rand()%8 == 0)
	{
		direction = rand()%4;
	}//if

	switch (direction)
	{
		case UP:
			y = -1;
			break;
		case DOWN:
			y = 1;
			break;
		case LEFT:
			x = -1;
			break;
		case RIGHT:
			x = 1;
			break;
		default:
			break;
	}//switch

	if (level->inBounds(position->getY()+y, position->getX()+x) && level->isPassable(position->getY()+y, position->getX()+x) && !level->isOccupied(position->getY()+y, position->getX()+x))
	{
		moved = true;
		level->removeCreature(this);
		position->setYX(position->getY()+y, position->getX()+x);	
		level->addCreature(this);
		if (level->getItem(position->getY(), position->getX()) != NULL && level->getItem(position->getY(), position->getX())->getType() == TRAP)
		{
			trap = (MouseTrap*)level->getItem(position->getY(), position->getX());
			trap->spring(this, level);
		}//if
	}//if

	return moved;
}//move

bool Creature::getHostile()
{
	return hostile;
}//getHostile

void Creature::setHostile(bool hostile)
{
	this->hostile = hostile;
}//setHostile

bool Creature::getType()
{
	return type;
}//getType

void Creature::setType(CreatureType type)
{
	this->type = type;
}//setType
/*** END CREATURE CLASS***/
