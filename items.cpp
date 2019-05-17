#include "items.h"

/*** ITEM CLASS***/
Item::Item()
{
	int i;

	this->symbol = 0;
	this->baseName = "";
	this->name = "";
	this->type = UNDEFINED;
	this->desc = "";
	this->ID = "ZZZ";

	for (i = 0; i < ITEMATTRIBS; i++)
		this->attribs[i] = 0;
}//Item

Item::Item(string name, char symbol, ItemType type, string desc, string ID)
{
	int i;

	this->baseName = name;
	this->name = name;
	this->symbol = symbol;
	this->type = type;
	this->desc = desc;
	this->ID = "" + ID;

	for (i = 0; i < ITEMATTRIBS; i++)
		this->attribs[i] = 0;
}//Item

Item::Item(Item* item)
{
	int i;

	this->baseName = item->getName();
	this->name = item->getName();
	this->symbol = item->getSymbol();
	this->type = item->getType();
	this->desc = item->getDescription();
	this->ID = "" + item->getID();

	for (i = 0; i < ITEMATTRIBS; i++)
		this->attribs[i] = item->getAttrib(i);
}//Item

Item* Item::itemFactory()
{
	if (getItemClass() == "Hammer")
		return new Hammer(this);
	else if (getItemClass() == "MouseTrap")
		return new MouseTrap(this);
	else
		return new Item(this);
}//itemFactory

bool Item::compare(Item* item)
{
	return (this->ID == item->getID());
	/*int i;
	for (i = 0; i < IDSIZE && equal; i++)
		equal = (this->ID[i] == item->getID()[i]);*/
}//compare

string Item::activate()
{
	/*if (type == TRAP)
	{
		attribs[0] = 1;
		return name + " activated!";
	}//if*/

	return "Nothing happens.";
}//activate

string Item::combine(Item* item, bool* combined)
{
	*combined = false;

	return "Nothing happens.";
}//combine

/*** GETTERS/SETTERS ***/
string Item::getID()
{
	return ID;
}//getID

int Item::getAttrib(int number)
{
	return this->attribs[number];
}//getAttrib

string Item::getBaseName()
{
	return this->baseName;
}//getBaseName

string Item::getName()
{
	return this->name;
}//getName

char Item::getSymbol()
{
	return this->symbol;
}//getSymbol

string Item::getDescription()
{
	return this->desc;
}//getDescription

ItemType Item::getType()
{
	return this->type;
}//getType

void Item::setAttrib(int number, int value)
{
	this->attribs[number] = value;
}//setAttrib

string Item::getItemClass()
{
	return "Item";
}//getItemClass
/*** END ITEM CLASS***/

/*** HAMMER CLASS ***/
Hammer::Hammer() : Item()
{
	wielded = false;
}//Hammer

Hammer::Hammer(string name, char symbol, ItemType type, string desc, string ID) : Item(name, symbol, type, desc, ID)
{
	wielded = false;
}//Hammer

Hammer::Hammer(Item* item) : Item(item)
{
	wielded = false;
}//Hammer

string Hammer::activate()
{
	return "A label reads: 'Swing to activate'";
}//activate

string Hammer::combine(Item* item, bool* combined)
{
	if (item->getID() == "GB1")
	{
		*combined = true;
		return "You smash the beaker when attempting to make the hammer fit...";
	}//if

	return "Why smash that?";
}//combine

string Hammer::getName()
{
	return this->name;
}//getName

ItemType Hammer::getType()
{
	return this->type;
}//getType

string Hammer::getItemClass()
{
	return "Hammer";
}//getItemClass
/*** END HAMMER CLASS ***/

/*** MOUSETRAP CLASS ***/
MouseTrap::MouseTrap() : Item()
{
	this->activated = false;
	this->baited = false;
}//MouseTrap

MouseTrap::MouseTrap(string name, char symbol, ItemType type, string desc, string ID) : Item(name, symbol, type, desc, ID)
{
	this->activated = false;
	this->baited = false;
}//MouseTrap

MouseTrap::MouseTrap(Item* item) : Item(item)
{
	this->activated = false;
	this->baited = false;
}//MouseTrap

string MouseTrap::activate()
{
	//this->setAttrib(0, 1);
	this->activated = true;
	return "Mousetrap is now ready to catch some unsuspecting rodents.";
}//activate

string MouseTrap::combine(Item* item, bool* combined)
{
	if (item->getType() == FOOD)
	{
		*combined = true;
		this->baited = true;
		return "You bait the mouse trap with the " + item->getName() + "!";
	}//if

	return "You can't bait the mousetrap with THAT!";
}//combine

string MouseTrap::spring(Creature* creature, Level* level)
{
	//if (creature->getType() == RODENT)
	//{
		level->removeCreature(creature);
	//}//if
}//spring

string MouseTrap::getName()
{
	this->name = this->baseName;

	if (activated)
		this->name += " (activated)";

	if (baited)
		this->name += " (baited)";

	return this->name;
}//getName

ItemType MouseTrap::getType()
{
	return this->type;
}//getType

string MouseTrap::getItemClass()
{
	return "MouseTrap";
}//getItemClass
/*** END MOUSETRAP CLASS ***/
