#include "player.h"
#include "list.h"
#include "coord.h"
#include "items.h"
#include "level.h"
#include "creature.h"

/*** PLAYER CLASS***/
Player::Player()
{
	weapon = NULL;
	this->title = "";
    this->inventory = new List();
}//Player

Player::Player(int str, int dex, int per, string name, Coord* position, char symbol, string title) : Agent(str, dex, per, name, position, symbol)
{
	weapon = NULL;
	this->title = title;
    this->inventory = new List();
}//Player

Player::~Player()
{
	/*int i;

	for (i = 0; i < itemCount; i++)
		delete inventory[i];*/
    delete this->inventory;
}//~Player

bool Player::addItem(Item* item)
{
	if (item != NULL)
	{
		//this->inventory[itemCount] = item->itemFactory();
		//itemCount++;
        this->inventory->push(item->itemFactory());
		return true;
	}//if

	return false;
}//addItem

Item* Player::removeItem(int number)
{
	/*int i;

	for (i = number; i < this->itemCount-1; i++)
		this->inventory[i] = this->inventory[i+1];

	itemCount--;*/
    return (Item*)this->inventory->remove(number);
}//removeItem

void Player::wieldWeapon(int number)
{
	if (weapon != NULL)
		unwieldWeapon();
	//weapon = inventory[number];
	weapon = (Item*)this->inventory->getPosition(number);
}//wieldWeapon

void Player::unwieldWeapon()
{
	weapon = NULL;
}//unwieldWeapon

bool Player::move(Coord* change, Level* level, string* banner)
{
	bool moved = false;
	int newY = this->position->getY() + change->getY();
	int newX = this->position->getX() + change->getX();

	if (level->inBounds(newY, newX) && level->isPassable(newY, newX) && !level->isOccupied(newY, newX))
	{
		updateLOS(level, true); //Reset visibilty for old LOS
		this->position->setYX(newY, newX);
		updateLOS(level, false); //Set visibility for new LOS
		moved = true;
	}//if
	else if (level->isOccupied(newY, newX) && level->getCreature(newY, newX) != NULL)
		*banner = "Your way is blocked by a " + level->getCreature(newY, newX)->getName() + ".";
	else if (!level->isPassable(newY, newX) && level->getItem(newY, newX) != NULL)
		*banner = "Your way is blocked by a " + level->getItem(newY, newX)->getName() + ".";

	delete change;

	return moved;
}//move

void Player::updateLOS(Level* level, bool reset)
{
	int quarter;
	int i, j;
	int newY, newX;
	int distOut;
	int distAcross;
	int distance = 4;

	for (quarter = 0; quarter < 4; quarter++)
	{
		distOut = distance;

		for (i = 0; i < distance; i++)
		{
			for (j = 1; j <= distOut; j++)
			{
				if (quarter == 0)
				{
					newY = position->getY() - j;
					newX = position->getX() + i;
				}//if
				else if (quarter == 1)
				{
					newY = position->getY() + i;
					newX = position->getX() + j;
				}//else if
				else if (quarter == 2)
				{
					newY = position->getY() + j;
					newX = position->getX() - i;
				}//else if
				else if (quarter == 3)
				{
					newY = position->getY() - i;
					newX = position->getX() - j;
				}//else if

				if (level->inBounds(newY, newX) && level->getType(newY, newX) != BEDROCK)
				{
					if (reset)
						level->notVisible(newY, newX);
					else
					{
						level->map(newY, newX);
						level->visible(newY, newX);

						if (level->getType(newY, newX) == VWALL || level->getType(newY, newX) == HWALL)
							j = 9999;

						if (quarter%2 == 0 && level->getType(newY, newX) == VWALL)
							i = 9999;
						else if (quarter%2 == 1 && level->getType(newY, newX) == HWALL)
							i = 9999;
					}//else
				}//if
			}//for

			distOut -= 1;
		}//for
	}//for

}//updateLOS

int Player::interest(Level* level, string* banner)
{
	int interest = 0;
	int i,j;

	for (i = -1; i <= 1; i++)
		for (j = -1; j <= 1; j++)
		{
			if (level->inBounds(position->getY()+i, position->getX()+j) && level->getItem(position->getY()+i, position->getX()+j) != NULL)
			{
				interest = 3;
				*banner = "You see a " + level->getItem(position->getY()+i, position->getX()+j)->getName() + ".";
			}//if
			else if (level->inBounds(position->getY()+i, position->getX()+j) && level->getCreature(position->getY()+i, position->getX()+j) != NULL)
			{
				interest = 3;
				*banner = "You see a " + level->getCreature(position->getY()+i, position->getX()+j)->getName() + ".";
			}//else if
		}//for	

	return interest;
}//interest

/*** GETTERS/SETTERS ***/
string Player::getTitle()
{
	return this->title;
}//getTitle

int Player::getItemCount()
{
	//return this->itemCount;
    return this->inventory->getCount();
}//getItemCount

Item* Player::getItem(int number)
{
	//return this->inventory[number];
	return (Item*)this->inventory->getPosition(number);
}//getItem

void Player::setTitle(string title)
{
	this->title = title;
}//setTitle

Item* Player::getWeapon()
{
	return this->weapon;
}//getWeapon
/*** END PLAYER CLASS***/


