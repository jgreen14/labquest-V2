#ifndef ROGUETEST_H
#define ROGUETEST_H

#define NUMLEVELS 20
#define MAXMONSTERS 15
#define MAXITEMS 15
#define MAXINVENTORY 30
#define LEVELWIDTH 78
#define LEVELHEIGHT 20

const int GENITEMS = 9;
const int NONGENITEMS = 2;

typedef enum
{
	WEAPON,
	MISSILE,
	ARMOR,
	POTION,
	SCROLL,
    ANY
} ItemType;

typedef enum
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
} Direction;

typedef struct
{
	char desc[80];
	char symbol;
	ItemType type;
	int weight;
	int attrib1;
	int attrib2;
	int attrib3;
} ItemDesc;

typedef struct
{
	int itemIndex;
	int y;
	int x;
} Item;

typedef struct
{
	int str;
	int intell;
	int dex;
	int exp;
	int hp;
	int alive;
	int y;
	int x;
} Monster;

typedef struct
{
	int y;
	int x;
	Direction dir;
} Stairs;

typedef struct
{
	int y;
	int x;
	int width;
	int height;
} Room;

typedef struct
{
    int visible;
    int passable;
    int occupied;
    int stairsUp;
    int stairsDown;
    char baseSymbol;
    char currentSymbol; 
} Tile;

typedef struct
{
	//char layout[LEVELHEIGHT][LEVELWIDTH];
	Tile layout[LEVELHEIGHT][LEVELWIDTH];
	//char map[LEVELHEIGHT][LEVELWIDTH];
	Monster monsters[MAXMONSTERS];
	int monsterCount;
	Item items[MAXITEMS];
	int itemCount;
	Stairs stairsUp;
	Stairs stairsDown;
	int stairsUpRoom;
	int stairsDownRoom;
	int number;
} Level;

typedef struct
{
	int str;
	int intell;
	int dex;
	int hp;
	int level;
	int exp;
	int turn;
	int y;
	int x;
	int itemCount;
	int items[MAXINVENTORY];
    ItemDesc *weapon;
    ItemDesc *armor;
} Player;

void removeItem(Level*, int, int);
int getItem(Level*, int, int);
void initItems(ItemDesc*);

void initLevel(Level*); 
void drawLevel(Level*);
void drawLOS(Level*, Player*);

int getMonster(Level*, int, int);
int monsterMove(Monster*, Level*, Player*);

void generateLevel(Level*, Level*, int);
void connectRooms(Room*, Room*, Direction, Level*);
void makeRoom(Level*, int, int, int, int);
void addMonster(Level*, int, int);
void addItem(Level*, int, int, int);

void drawBorder();
void printStats(Player*, int);
void addPlayerItem(Player*, int);
void playerAttack(Player*, Monster*, ItemDesc*);
void monsterAttack(Monster*, Player*);
void levelUp(Player*);
void displayInventory(Player*, ItemDesc*);
void useItem(Player*, ItemType, ItemDesc*);
void removePlayerItem(Player*, int);

void updateTile(Tile*, char, char, int, int, int);
#endif
