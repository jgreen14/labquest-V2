#include "creature.h"
#include "coord.h"
#include "items.h"
#include "level.h"

/*** CREATURE CLASS***/
Creature::Creature() : Agent()
{
	hostile = true;
	direction = Direction::STOPPED;
	type = CreatureType::UNKNOWN;
}//Creature

Creature::Creature(int str, int dex, int per, string name, Coord* position, char symbol) : Agent(str, dex, per, name, position, symbol)
{
	hostile = true;
	direction = Direction::STOPPED;
	type = CreatureType::UNKNOWN;
}//Creature

Creature::Creature(int str, int dex, int per, string name, Coord* position, char symbol, CreatureType type) : Agent(str, dex, per, name, position, symbol)
{
	hostile = true;
	direction = Direction::STOPPED;
	this->type = type;
}//Creature

bool Creature::move(Level* level)
{
	bool moved = false;
	int x = 0, y = 0;
	MouseTrap* trap;

	if (direction == Direction::STOPPED || rand()%8 == 0)
	{
		direction = static_cast<Direction>(rand()%4);
	}//if

	switch (direction)
	{
		case Direction::UP:
			y = -1;
			break;
		case Direction::DOWN:
			y = 1;
			break;
		case Direction::LEFT:
			x = -1;
			break;
		case Direction::RIGHT:
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
		if (level->getItem(position->getY(), position->getX()) != NULL && level->getItem(position->getY(), position->getX())->getType() == ItemType::TRAP)
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

CreatureType Creature::getType()
{
	return type;
}//getType

void Creature::setType(CreatureType type)
{
	this->type = type;
}//setType
/*** END CREATURE CLASS***/
