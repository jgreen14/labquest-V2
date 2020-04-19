#ifndef LABQUEST_H
#define LABQUEST_H

#include <string>
#include <stdlib.h>

constexpr int LEVELHEIGHT{ 21 };
constexpr int LEVELWIDTH{ 78 };
constexpr int ITEMATTRIBS{ 10 };
constexpr int IDSIZE{ 3 };
constexpr int NUMLEVELS{ 20 };
constexpr int MAXITEMS{ 100 };
constexpr int INVENTORYSIZE{ 50 };

enum class GameState
{
	WAIT,
	MOVE,
	PICKUP,
	DROP,
	INVENTORY,
	NEXTLEVEL,
	PREVLEVEL
};

enum class ItemType
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
};

enum class TileType
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
};

enum class Direction
{
	STOPPED = -1,
	UP,
	DOWN,
	LEFT,
	RIGHT
};

enum class CreatureType
{
	UNKNOWN = -1,
	RODENT,
    HUMAN,
    MUTANT
};

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
