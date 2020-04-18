#ifndef LABQUEST_H
#define LABQUEST_H

#include <string>
#include <stdlib.h>

#define LEVELHEIGHT 21
#define LEVELWIDTH 78
#define ITEMATTRIBS 10
#define IDSIZE 3
#define NUMLEVELS 20

const int MAXITEMS = 100;
const int INVENTORYSIZE = 50;

typedef enum
{
	UNDEFINED = -1,
	WEAPON,
	TRAP,
	ARMOR,
	NOTE,
	BOOK,
	CHEMICAL,
	CONTAINER,
	DETECTOR,
	EQUIPMENT,
	FOOD,
	FURNITURE
} ItemType;

typedef enum
{
	NONE = -1,
	BEDROCK,
	VWALL,
	HWALL,
	ULCORNER,
	URCORNER,
	LLCORNER,
	LRCORNER,
	FLOOR,
	CORRIDOR,
	WATER,
	ACID,
	ORE,
    UPSTAIRS,
    DOWNSTAIRS
} TileType;

typedef enum
{
	STOPPED = -1,
	UP,
	DOWN,
	LEFT,
	RIGHT
} Direction;

typedef enum
{
	UNKNOWN = -1,
	RODENT,
    HUMAN,
    MUTANT
} CreatureType;

class List;
class Coord;
class Item;
class Tile;
class Room;
class Level;
class Agent;
class Player;
class Creature;
class IO;

#endif
