#ifndef LEVEL_H
#define LEVEL_H

#include "labquest.h"

using namespace std;

class Level
{
    private:
        Tile* layout[LEVELHEIGHT][LEVELWIDTH];
        Coord* upStairs;
        Coord* downStairs;
        int levelNum;

    public:
        Level(int);

		//void generateLab(Item*[], int);
        void generateLevel(Level*);
        void connectRooms(Room*, Room*, Direction);
        void makeRoom(int, int, int, int);
		bool isPassable(int, int);
		bool isOccupied(int, int);
		bool isMapped(int, int);
		bool isVisible(int, int);
		void map(int, int);
		void visible(int, int);
		void notVisible(int, int);
		string getTileInfo(Coord*);
		bool addItem(Coord*, Item*);
		bool addCreature(Creature*);
		bool removeCreature(Creature*);
		bool inBounds(int, int);
		Item* itemPickup(Coord*);
        TileType getType(int, int);
		Item* getItem(int, int);
		Creature* getCreature(int, int);
        Coord* getUpStairsCoord();
        Coord* getDownStairsCoord();
};//Level class

#endif
