#ifndef PLAYER_H
#define PLAYER_H

#include "labquest.h"
#include "agent.h"

using namespace std;

class Player : public Agent
{
	private:
		string title;
		//Item* inventory[INVENTORYSIZE];
        List* inventory;
		//int itemCount;
		Item* weapon;

	public:
		Player();
		Player(int, int, int, string, Coord*, char, string);
		~Player();

		bool addItem(Item*);
		Item* removeItem(int);
		void wieldWeapon(int);
		void unwieldWeapon();
		void updateLOS(Level*, bool);
		bool move(Coord*, Level*, string*);
		int interest(Level*, string*);

		string getTitle();
		int getItemCount();
		Item* getItem(int);
		void setTitle(string);
		Item* getWeapon();
};//Player class

#endif
