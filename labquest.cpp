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
	int i;
	int itemCount = 0;
    List* allItems = new List();
	List* allLevels = new List();
	IO* io = new IO();
	Creature* creature;
	Level* currLevel = (Level*)allLevels->getHead();
	Level* prevLevel = NULL;
	GameState currState = WAIT;

	srand(time(NULL));

	itemCount = populateItems(allItems);

	// Generate levels
	for (i = 0; i < NUMLEVELS; i++) {
		allLevels->push(new Level(i));
		currLevel = (Level*)allLevels->getTail();
		currLevel->generateLevel(prevLevel);
		
		// Add test creature and item to each level
		// **Should have populateItems and populateCreatures functions
		creature = new Creature(1, 1, 1, "Rat", new Coord(1, 20), 'r', RODENT);
		currLevel->addCreature(creature);
		currLevel->addItem(new Coord(2, 25), ((Item*)allItems->getPosition(2))->itemFactory());
		
		prevLevel = currLevel;
	} // for

	// Reset level pointers
	// **This should be done in some sort of initGame function to handle loading save games too
	currLevel = (Level*)allLevels->getHead();
	prevLevel = NULL;

	// Setup player and add test item
	Player* player = new Player(1, 1, 1, "Jeff", currLevel->getUpStairsCoord(), '@', "Novice");
	player->addItem(((Item*)allItems->getPosition(0))->itemFactory());
	
	// Update LOS before start so player's starting view is shown
	player->updateLOS(currLevel, false);

	while (!io->halt())
	{
		io->drawBorder();
		io->showLevel(currLevel);
		io->showAgent(player);
		// io->clearBanner();
		io->printBanner();
		io->printStats(player);
		io->refresh();

		io->readInput(true);
		currState = io->processInput(player, currLevel);
		// Game logic should be moved to it's own class
		switch (currState) 
		{
			// **Should have a changeLevel method that does more checks and setup
			case NEXTLEVEL:
				if (currLevel->getDownStairsCoord()->equal(player->getPosition()) && currLevel->getNext() != NULL)
				{
					prevLevel = currLevel;
					currLevel = (Level*)currLevel->getNext();
					player->updateLOS(currLevel, false);
				} // if
				break;
			case PREVLEVEL:
				if (currLevel->getUpStairsCoord()->equal(player->getPosition()) && prevLevel != NULL)
				{
					// **Could have a list of visited levels to make this easier
					currLevel = prevLevel;
					prevLevel = (Level*)allLevels->getPosition(currLevel->getValue() - 1);
					player->updateLOS(currLevel, false);
				} // if
				break;
			default:
				break;
		}

		// Need an updateLevel function that does all creature/object moves
		// creature->move(currLevel);
	}//while
    
    io->close();

    return 0;
}//main
