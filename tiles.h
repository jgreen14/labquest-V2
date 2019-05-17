#ifndef TILES_H
#define TILES_H

#include "labquest.h"
#include "list.h"

using namespace std;

class Tile
{
    private:
        bool occupied;
        bool visible;
        bool passable;
        bool mapped;
		TileType type;
        List* items;
		Creature* creature;

   public:
        Tile(TileType);

		Item* takeItem();
		TileType getType();
		void setType(TileType);
        bool getOccupied();
        void setOccupied(bool);
        bool getVisible();
        void setVisible(bool);
        bool getPassable();
        void setPassable(bool);
        bool getMapped();
        void setMapped(bool);
		Item* getFirstItem();
		void addItem(Item*);
		Creature* getCreature();
		void setCreature(Creature*);
};//Tile class

#endif
