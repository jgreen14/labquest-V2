#include <iostream>
#include <time.h>

#include "labquest.h"
#include "list.h"
#include "coord.h"
#include "items.h"
#include "tiles.h"
#include "room.h"
#include "level.h"
#include "player.h"
#include "creature.h"
#include "io.h"

using namespace std;

int populateItems(List* items)
{
	items->push(new Hammer("Hammer", ')', WEAPON, "A regular wood handled hammer.", "WH1"));
	items->push(new Item("Table", '^', FURNITURE, "A rectangular work table.", "WT1"));
	items->push(new Item("Scrawled Note", '?', NOTE, "'...I have nearly completed my latest experiments... but I keep running into problems...'", "SN1"));
	items->push(new Item("Dusty Book", '+', BOOK, "'Chapter 1 - The most important part to remember when building traps is that placement is everything.'", "DB1"));
	items->push(new Item("Beaker", 'v', CONTAINER, "An ordinary glass beaker.", "GB1"));
	items->push(new MouseTrap("Mousetrap", 'T', TRAP, "A trap suitable for catching regular sized mice.", "MT1"));
	items->push(new Item("Motion Sensor", '*', DETECTOR, "Can sense even very minute motions in a sizeable radius around the user.", "MS1"));
	items->push(new Item("Microscope", '!', EQUIPMENT, "Up to 100X zoom! Very handy for examining the cellular or crystalline structure of various substances.", "EM1"));
	items->push(new Item("Cheese", '%', FOOD, "Delicious cheese!", "FC1"));
	items->push(new Item("Apple", '%', FOOD, "A shiny, red apple.", "FA1"));

	return items->getCount();
}//populateItems

int main()
{
	int itemCount = 0;
    List* allItems = new List();
	IO* io = new IO();
	//Player* player = new Player(1, 1, 1, "Jeff", new Coord(0, 0), '@', "Novice");
	Creature* creature = new Creature(1, 1, 1, "Rat", new Coord(1, 20), 'r', RODENT);
    Level* level0 = new Level(0);
    Level* level1 = new Level(1);

	srand(time(NULL));

	itemCount = populateItems(allItems);
	//level->generateLab(allItems, itemCount);
    level0->generateLevel(NULL);
    level1->generateLevel(level0);
	Player* player = new Player(1, 1, 1, "Jeff", level0->getUpStairsCoord(), '@', "Novice");
	level0->addCreature(creature);
	player->addItem(((Item*)allItems->getPosition(0))->itemFactory());

	cout << "Player name: " << player->getName() << endl;
	cout << "Item count: " << allItems->getCount() << endl;

	/* while (!TCODConsole::isWindowClosed()) {
		TCOD_key_t key;
		TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS, &key, NULL);
		if (key.c == 'f')
			TCODConsole::setFullscreen(!TCODConsole::isFullscreen());
		TCODConsole::root->clear();
		TCODConsole::root->putChar(40, 25, '@');
		TCODConsole::flush();
	} */
	
	// Update LOS before start so player's starting view is shown
	player->updateLOS(level0, false);

	io->readInput(false);
	while (!io->halt())
	{
		io->processInput(player, level0);
		creature->move(level0);
		io->printBanner();
		io->drawBorder();
		io->showLevel(level0);
		io->showAgent(player);
		io->printStats(player);
		io->refresh();

		io->readInput(true);
	}//while
    
    io->close();

    return 0;
}//main
