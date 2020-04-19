#include <iostream>
#include <time.h>
#include <assert.h>

#include "labquest.h"
#include "list.h"
#include "coord.h"
#include "items.h"
#include "tiles.h"
#include "room.h"
#include "level.h"
#include "player.h"
#include "creature.h"

constexpr int itemCount{ 2 };
const string playerName{ "Jeff" };

using namespace std;

int populateItems(List* items)
{
	items->push(new Hammer("TestWeapon1", ')', ItemType::WEAPON, "A test weapon.", "TW1"));
	items->push(new Item("TestFurniture1", '^', ItemType::FURNITURE, "A test piece of furniture.", "TF1"));
	return items->getCount();
}//populateItems

int main()
{
	int itemCount = 0;
	List* allItems = new List();
	//Player* player = new Player(1, 1, 1, "Jeff", new Coord(0, 0), '@', "Novice");
	Creature* creature = new Creature(1, 1, 1, "Rat", new Coord(1, 20), 'r', CreatureType::RODENT);
	Level* level0 = new Level(0);
	Level* level1 = new Level(1);

	srand(time(NULL));

	itemCount = populateItems(allItems);
	//level->generateLab(allItems, itemCount);
	level0->generateLevel(NULL);
	level1->generateLevel(level0);
	Player* player = new Player(1, 1, 1, playerName, level0->getUpStairsCoord(), '@', "Novice");
	//level->addCreature(creature);
	player->addItem(((Item*)allItems->getPosition(0))->itemFactory());

	assert(player->getName() == playerName && "Player name not set correctly");
	assert(allItems->getCount() == itemCount && "Incorrect number of items");

	cout << "All tests passed!" << endl;

	return 0;
} // main