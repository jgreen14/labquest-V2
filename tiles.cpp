#include "tiles.h"
#include "items.h"
#include "creature.h"

/*** TILE CLASS ***/
Tile::Tile(TileType type)
{
	occupied = false;
	visible = false;
	passable = false;
	mapped = false;
	this->type = type;
    items = new List();
	//creature = NULL;
}//Tile;

Item* Tile::takeItem()
{
    return (Item*)items->shift();
}//removeItem

/*** GETTERS/SETTERS ***/
TileType Tile::getType()
{
	return this->type;
}//getType

void Tile::setType(TileType type)
{
	this->type = type;
}//setType

void Tile::setOccupied(bool value)
{
	this->occupied = value;
}//setOccupied

bool Tile::getOccupied()
{
	return this->occupied;
}//setCurrentSymbol

Creature* Tile::getCreature()
{
	return this->creature;
}//getCreature

void Tile::setCreature(Creature* creature)
{
	this->creature = creature;
}//setCreature

Item* Tile::getFirstItem()
{
    return (Item*)items->getHead();
}//getFirstItem

void Tile::addItem(Item* item)
{
    items->push(item->itemFactory());
}//setItem

void Tile::setVisible(bool value)
{
	this->visible = value;
}//setOccupied

bool Tile::getVisible()
{
	return this->visible;
}//setCurrentSymbol

void Tile::setPassable(bool value)
{
	this->passable = value;
}//setOccupied

bool Tile::getPassable()
{
	return this->passable;
}//setCurrentSymbol

void Tile::setMapped(bool value)
{
	this->mapped = value;
}//setOccupied

bool Tile::getMapped()
{
	return this->mapped;
}//setCurrentSymbol

/*** END TILE CLASS ***/
