#ifndef ITEMS_H
#define ITEMS_H

#include "labquest.h"
#include "level.h"
#include "creature.h"
#include "node.h"

using namespace std;

class Item : public Node
{
	private:
		string desc;
		char symbol;
		string ID;
		int attribs[ITEMATTRIBS];

	protected:
		string baseName;
		string name;
		ItemType type;

	public:
		Item();
		Item(Item*);
		Item(string, char, ItemType, string, string);

		Item* itemFactory();

		virtual string activate();
		virtual string combine(Item*, bool*);
		bool compare(Item*);

		virtual string getName();
		virtual ItemType getType();
		virtual string getItemClass();
		string getBaseName();
		int getAttrib(int);
		char getSymbol();
		string getDescription();
		void setAttrib(int, int);
		string getID();
};//Item class

class Hammer : public Item
{
	private:
		bool wielded;

	public:
		Hammer();
		Hammer(string, char, ItemType, string, string);
		Hammer(Item*);
		
		string activate();
		string combine(Item*, bool*);

		string getName();
		ItemType getType();
		string getItemClass();
};//Hammer class

class MouseTrap : public Item
{
	private:
		bool activated;
		bool baited;

	public:
		MouseTrap();
		MouseTrap(string, char, ItemType, string, string);
		MouseTrap(Item*);

		string activate();
		string combine(Item*, bool*);
		string spring(Creature*, Level*);

		string getName();
		ItemType getType();
		string getItemClass();
};//MouseTrap class

#endif
