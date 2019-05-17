#include "io.h"
#include "items.h"
#include "coord.h"
#include "level.h"
#include "player.h"
#include "creature.h"
#include <ncurses.h>

/*** IO CLASS ***/
IO::IO()
{
	initscr();
	curs_set(0);
	clear();
	noecho();
	cbreak();
	keypad(stdscr, TRUE);
	this->banner = "";
	this->input = 0;
}//Output

void IO::drawBorder()
{
	int i, j;

	for (i = 0; i < LEVELHEIGHT+2; i++)
		for (j = 0; j < LEVELWIDTH+2; j++)
			mvaddch(i+1, j, '*');
}//drawBorder

void IO::showLevel(Level* level)
{
	Coord* pos;
	Creature* creature;
	int i, j;

	for (i = 0; i < LEVELHEIGHT; i++)
		for (j = 0; j < LEVELWIDTH; j++)
		{
			pos = new Coord(i+2, j+1);

			if (level->isVisible(i, j) && (creature = level->getCreature(i, j)) != NULL)
			{
				showAgent(creature);
				//level->getCreature(i, j)->move(level);
				//showAgent(level->getCreature(i, j));
			}//if
			else if (level->isMapped(i, j) && level->getItem(i, j) != NULL)
				mvaddch(pos->getY(), pos->getX(), level->getItem(i, j)->getSymbol());
			else if (level->isMapped(i, j))
			{
				switch(level->getType(i, j))
				{
					case VWALL:
						mvaddch(pos->getY(),pos->getX(),ACS_VLINE);
						break;
					case HWALL:
						mvaddch(pos->getY(),pos->getX(),ACS_HLINE);
						break;
					case ULCORNER:
						mvaddch(pos->getY(),pos->getX(),ACS_ULCORNER);
						break;
					case URCORNER:
						mvaddch(pos->getY(),pos->getX(),ACS_URCORNER);
						break;
					case LLCORNER:
						mvaddch(pos->getY(),pos->getX(),ACS_LLCORNER);
						break;
					case LRCORNER:
						mvaddch(pos->getY(),pos->getX(),ACS_LRCORNER);
						break;
					case CORRIDOR:
						mvaddch(pos->getY(),pos->getX(),ACS_BLOCK);
						break;
					case FLOOR:
						mvaddch(pos->getY(),pos->getX(),'.');
						break;
					case WATER:
						mvaddch(pos->getY(),pos->getX(),'~');
						break;
                    case UPSTAIRS:
						mvaddch(pos->getY(),pos->getX(),'<');
						break;
                    case DOWNSTAIRS:
						mvaddch(pos->getY(),pos->getX(),'>');
						break;
					default:
						mvaddch(pos->getY(),pos->getX(),'?');
						break;
				}//switch

			}//else if
			else
				mvaddch(pos->getY(),pos->getX(),' ');
		}//for

	delete pos;
}//showLevel

void IO::showAgent(Agent* agent)
{
	int y = agent->getPosition()->getY();
	int x = agent->getPosition()->getX();

	mvaddch(y+2, x+1, agent->getSymbol());
}//showAgent

void IO::showInventoryScreen(Player* player)
{
	int i;
	char response;

	clear();

	mvprintw(0, 0, "Inventory:");
	for (i = 0; i < player->getItemCount(); i++)
	{
		if (player->getWeapon() == player->getItem(i))
			mvprintw(i+1, 0, "%c. %s (wielded)", i+97, player->getItem(i)->getName().c_str());
		else
			mvprintw(i+1, 0, "%c. %s", i+97, player->getItem(i)->getName().c_str());
	}//for
	response = getch();
	if (response >= 97 && response < i + 97)
	{
		clear();
		mvprintw(0, 0, "%s", player->getItem(response-97)->getDescription().c_str());
		getch();
	}//if

	clear();
}//showInventoryScreen

void IO::showActivateScreen(Player* player)
{
	int i;
	char response;

	clear();

	mvprintw(0, 0, "Choose item to activate:");
	for (i = 0; i < player->getItemCount(); i++)
		mvprintw(i+1, 0, "%c. %s", i+97, player->getItem(i)->getName().c_str());
	response = getch();

	if (response >= 97 && response < i + 97)
	{
		//banner = "Activated a " + player->getItem(response-97)->getName() + ".";
		banner = player->getItem(response-97)->activate();
	}//if

	clear();
}//showActivateScreen

void IO::showCombineScreen(Player* player)
{
	int i, count = 0;
	char response;
	int choices[2];
	bool combined = false;

	clear();

	mvprintw(0, 0, "Choose 2 items to combine (then press Enter):");
	for (i = 0; i < player->getItemCount(); i++)
		mvprintw(i+1, 0, "%c. %s", i+97, player->getItem(i)->getName().c_str());
	response = getch();

	while (response != '\n')
	{
		if (response >= 97 && response < i + 97)
		{
			mvprintw(response - 96, 0, "+ %c. %s", response, player->getItem(response-97)->getName().c_str());
			choices[count] = response - 97;
			count++;
		}//if

		response = getch();
	}//while
	
	if (choices[0] == choices[1])
		banner = "Can't combine an item with itself!";
	else if (count > 1)
	{
		//Swap items so that lowest type is first (item with the lowest type should ALWAYS be the item combine() is called on)
		if (player->getItem(choices[0])->getType() > player->getItem(choices[1])->getType())
		{
			i = choices[0];
			choices[0] = choices[1];
			choices[1] = i;
		}//if

		banner = player->getItem(choices[0])->combine(player->getItem(choices[1]), &combined);

		if (combined)
			player->removeItem(choices[1]);
	}//if
	else
		banner = "Combination cancelled.";

	clear();
}//showCombineScreen

void IO::showWieldScreen(Player* player)
{
	int i;
	char response;

	clear();

	mvprintw(0, 0, "Choose weapon to wield:");
	for (i = 0; i < player->getItemCount(); i++)
	{
		if (player->getItem(i)->getType() == WEAPON)
			mvprintw(i+1, 0, "%c. %s", i+97, player->getItem(i)->getName().c_str());
	}//for
	response = getch();
	if (response >= 97 && response < i + 97)
	{
		banner = "";
		if (player->getWeapon() != NULL)
			banner = "Unwielding a " + player->getWeapon()->getName() + ". ";
		banner += "Wielding a " + player->getItem(response-97)->getName() + ".";
		player->wieldWeapon(response-97);
	}//if

	clear();
}//showWieldScreen

void IO::showDropScreen(Player* player, Level* level)
{
	int i;
	char response;

	clear();

	mvprintw(0, 0, "Drop Item:");
	for (i = 0; i < player->getItemCount(); i++)
		mvprintw(i+1, 0, "%c. %s", i+97, player->getItem(i)->getName().c_str());
	response = getch();
	if (response >= 97 && response < i + 97)
	{
        banner = "Dropped a " + player->getItem(response-97)->getName() + ".";
        level->addItem(player->getPosition(), player->removeItem(response-97));
	}//if

	clear();
}//showScreen

void IO::examineTile(Coord* position, Level* level)
{
	Coord* newCoord;

	banner = "Which direction do you want to look? (Press key to look in that direction)";
	printBanner();
	readInput();

	clear();
	mvprintw(0, 0, "You See:");
	newCoord = changeCoord();
	newCoord->setYX(newCoord->getY()+position->getY(), newCoord->getX()+position->getX());
	mvprintw(1,0, level->getTileInfo(newCoord).c_str());
	getch();

	clear();
}//examineTile

void IO::printBanner()
{
	mvprintw(0,0, "                                                                                ");
	mvprintw(0,0, banner.c_str());
}//printBanner

void IO::printStats(Player* player)
{
	string stats = player->getName() + " the " + player->getTitle() + " Str:" + (char)(player->getStr()+48) + " Dex:" + (char)(player->getDex()+48) + " Per:" + (char)(player->getPer()+48) + " Items:" + (char)(player->getItemCount()+48);

	mvprintw(24, 0, stats.c_str());
}//printStats

void IO::close()
{
	curs_set(1);
	echo();
	endwin();
}//close

void IO::readInput()
{
	this->input = getch();
}//readInput

void IO::fastMove(Player* player, Level* level)
{
	bool stop = false;

	nodelay(stdscr, TRUE);
	while (!stop && player->move(changeCoord(), level, &banner) && player->interest(level, &banner) < 3)
	{
		showLevel(level);	
		showAgent(player);
		delay_output(3000);
		refresh();
		if (getch() != ERR)
			stop = true;
	}//while
	nodelay(stdscr, FALSE);
}//fastMove

void IO::processInput(Player* player, Level* level)
{
	banner = "";

	switch(this->input)
	{
		case 'h':
		case 'j':
		case 'k':
		case 'l':
		case 'y':
		case 'u':
		case 'b':
		case 'n':
			player->move(changeCoord(), level, &banner);
			break;
		case 'H':
		case 'J':
		case 'K':
		case 'L':
		case 'Y':
		case 'U':
		case 'B':
		case 'N':
			fastMove(player, level);
			break;
		case ',':
			if (player->addItem(level->itemPickup(player->getPosition())))
				banner = "Picked up " + player->getItem(player->getItemCount()-1)->getName() + ".";
			break;
		case 'd':
			showDropScreen(player, level);
			break;
		case 'i':
			showInventoryScreen(player);
			break;
		case 'a':
			showActivateScreen(player);
			break;
		case 'w':
			showWieldScreen(player);
			break;
		case 'c':
			showCombineScreen(player);
			break;
		case 'x':
		case ';':
			examineTile(player->getPosition(), level);
			break;
		default:
			break;
	}//switch

}//processInput

Coord* IO::changeCoord()
{
	Coord* change = new Coord(0, 0);

	switch (input)
	{
		case 'h':
		case 'H':
			change->setX(-1);
			break;
		case 'j':
		case 'J':
			change->setY(1);
			break;
		case 'k':
		case 'K':
			change->setY(-1);
			break;
		case 'l':
		case 'L':
			change->setX(1);
			break;
		case 'y':
		case 'Y':
			change->setY(-1);
			change->setX(-1);
			break;
		case 'u':
		case 'U':
			change->setY(-1);
			change->setX(1);
			break;
		case 'b':
		case 'B':
			change->setY(1);
			change->setX(-1);
			break;
		case 'n':
		case 'N':
			change->setY(1);
			change->setX(1);
			break;
		default:
			break;
	}//switch

	return change;
}//changeCoord

char IO::getInput()
{
	return input;
}//getInput

bool IO::halt()
{
	if (this->input == 'q')
		return true;
	else
		return false;
}//halt

/*** END IO CLASS ***/
