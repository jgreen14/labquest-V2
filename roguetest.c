#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <time.h>

#include "roguetest.h"

char bannerMsg[80];

int main (int argc, char* argv[])
{
	int dx, dy, rely, relx;
	int i;
	char input;
	int updateAll = 0;
	int currentItem;
	int dungeonLevel = 0;
	Level levels[NUMLEVELS];
	Level *currentLevel = &levels[dungeonLevel];
	Player player = {3, 3, 3, 12, 1, 0, 0, 12, 24, 0};
	ItemDesc allItems[50];

	srand(time(NULL));

    //ncurses initialization
    initscr();
    curs_set(0);
	clear();
	noecho();
    cbreak();

    //Populate the master item list
	initItems(allItems);

    //Add player and give starting equipment
    addPlayerItem(&player, 0); //sword
    addPlayerItem(&player, 1); //armour
    player.weapon = &allItems[0];
    player.armor = &allItems[1];

    //Generate all the levels
	for (i = 0; i < NUMLEVELS; i++)
	{
		generateLevel(&levels[i], &levels[i-1], i);
	}//for
	
    //Start player on stairs up to "level 0"
	player.y = currentLevel->stairsUp.y+2;
	player.x = currentLevel->stairsUp.x+1;

    //Set the item symbols
	for (i = 0; i < currentLevel->itemCount; i++)
	{
		currentLevel->layout[currentLevel->items[i].y][currentLevel->items[i].x].currentSymbol =
            allItems[currentLevel->items[i].itemIndex].symbol;
	}//for

    //Set the monster symbols
	for (i = 0; i < currentLevel->monsterCount; i++)
	{
		currentLevel->layout[currentLevel->monsters[i].y][currentLevel->monsters[i].x].currentSymbol = 'M';
	}//for

	drawBorder();
    drawLOS(currentLevel, &player);
	drawLevel(currentLevel);
	mvaddch(player.y, player.x, '@');
	printStats(&player, dungeonLevel);
	refresh();

    //Get player position relative to top left corner of level (instead of the top level corner of the screen)
	rely = player.y - 2;
	relx = player.x - 1;

    //Read player input until player dies or eXits the game
	while ((input = getch()) != 'X' && player.hp > 0)
	{
        dx = 0;
        dy = 0;
		//mvaddch(player.y, player.x, ' ');
		snprintf(bannerMsg, 80, "                                                                                 ");

		switch(input)
		{
            //Movement inpu
			case 'h':
                dx = -1;
				break;
			case 'j':
                dy = 1;
				break;
			case 'k':
                dy = -1;
				break;
			case 'l':
                dx = 1;
				break;
			case 'y':
                dx = -1;
                dy = -1;
				break;
			case 'u':
                dx = 1;
                dy = -1;
				break;
			case 'b':
                dx = -1;
                dy = 1;
				break;
			case 'n':
                dx = 1;
                dy = 1;
				break;
            //Inventory input
			case 'i':
				//displayInventory(&player, allItems);
				useItem(&player, ANY, allItems);
				break;
			case 'q':
				useItem(&player, POTION, allItems);
				break;
			case 'r':
				useItem(&player, SCROLL, allItems);
				break;
            //Stairs up/down input
			case '>':
				if (currentLevel->stairsDown.y == rely && currentLevel->stairsDown.x == relx)//layout[rely][relx] == '>')
				{
					dungeonLevel += 1;
					currentLevel = &levels[dungeonLevel];
				}//if
				else
					snprintf(bannerMsg, 80, "No stairs here!");
				break;
			case '<':
                if (currentLevel->stairsUp.y == rely && currentLevel->stairsUp.x == relx)//layout[rely][relx] == '>')
				{
                    if (dungeonLevel > 0)
                    {
					    dungeonLevel -= 1;
					    currentLevel = &levels[dungeonLevel];
                    }//if
				}//if
				else
					snprintf(bannerMsg, 80, "No stairs here!");
				break;
            //Pick up item input
			case ',':
				currentItem = getItem(currentLevel, rely, relx);
				if (currentItem > -1)
				{
					removeItem(currentLevel, rely, relx);
					addPlayerItem(&player, currentItem);
					currentLevel->layout[rely][relx].currentSymbol = '.';
					snprintf(bannerMsg, 80, "Picked up %s.", allItems[currentItem].desc);
				}//if
			default:
				break;
		}//switch

        //If player is moving check if moving into a monster and new tile is passable
        if (dx != 0 || dy != 0)
        {
            if (getMonster(currentLevel, rely+dy, relx+dx)  != -1)
                playerAttack(&player, &currentLevel->monsters[getMonster(currentLevel, rely+dy, relx+dx)], allItems);
            else if (rely+dy >= 0 && rely+dy < LEVELHEIGHT && relx+dx >= 0 && relx+dx < LEVELWIDTH &&
                        currentLevel->layout[rely+dy][relx+dx].passable && !currentLevel->layout[rely+dy][relx+dx].occupied)
            {
                player.x += dx;
                player.y += dy;
                relx = player.x - 1;
                rely = player.y - 2;
            }//if
            player.turn++;
        }//if

        //Set all the items to be displayed
		for (i = 0; i < currentLevel->itemCount; i++)
		{
			currentLevel->layout[currentLevel->items[i].y][currentLevel->items[i].x].currentSymbol = allItems[currentLevel->items[i].itemIndex].symbol;
		}//for

        //Move monsters (or remove if dead)
		for (i = 0; i < currentLevel->monsterCount; i++)
		{
			if (currentLevel->monsters[i].hp > 0)
            {
                monsterMove(&currentLevel->monsters[i], currentLevel, &player);
            }//if
			else if (currentLevel->monsters[i].alive == 1)
			{
				player.exp += currentLevel->monsters[i].exp;
                levelUp(&player);
				currentLevel->layout[currentLevel->monsters[i].y][currentLevel->monsters[i].x].currentSymbol = '.';
				currentLevel->monsters[i].alive = 0;
			}//else
		}//for

	    clear();
		drawBorder();
        drawLOS(currentLevel, &player);
		drawLevel(currentLevel);
		updateAll -= 1;

		mvprintw(0,0, "                                                                                ");
		mvprintw(0,0, bannerMsg);
		mvaddch(player.y, player.x, '@');
		printStats(&player, dungeonLevel);
		refresh();
	}//while
		
    curs_set(1);
	endwin();

	return 0;
}//main

/*** MAIN GAME FUNCTIONS ***/
void drawBorder()
{
	int i = 0;

	while (i < LEVELWIDTH+2)
	{
		mvaddch(1, i, '*');	
		mvaddch(LEVELHEIGHT+2, i, '*');	
		i++;
	}//while

	i = 2;
	while (i < LEVELHEIGHT+3)
	{
		mvaddch(i, 0, '*');	
		mvaddch(i, LEVELWIDTH+1, '*');
		i++;
	}//while
}//drawBorder
/*** END MAIN GAME FUNCTIONS ***/

/*** PLAYER FUNCTIONS ***/
void printStats(Player *player, int dungeonLevel)
{
	mvprintw(23, 0, "HP:%d Str:%d Int:%d Dex:%d\tLev:%d\tExp:%d\tDungeon:%d\tTurn:%d", player->hp, player->str, player->intell, player->dex, player->level, player->exp, dungeonLevel+1, player->turn);
}//printStats

void addPlayerItem(Player* player, int item)
{
	if (player->itemCount < MAXITEMS-1)
	{
		player->items[player->itemCount] = item;
		player->itemCount++;
	}//if
}//addPlayerItem

void playerAttack(Player* player, Monster *monster, ItemDesc *allItems)
{
	int damage;

	if ((player->dex + rand()%10 + player->weapon->attrib1) > (monster->dex + rand()%10))
	{
		damage = (rand() % (player->str*2)) + 1 + player->weapon->attrib2;
		monster->hp -= damage;
		if (monster->hp <= 0)
		{
			snprintf(bannerMsg, 80, "Killed monster!");
		}//if
		else
			snprintf(bannerMsg, 80, "Hit monster for %d damage.", damage);
	}//if
	else
		snprintf(bannerMsg, 80, "Missed monster.");

	if (monster->hp > 0)
		monsterAttack(monster, player);
}//playerAttack*/

void monsterAttack(Monster *monster, Player *player)
{
	int damage;
    char tempString[80];

    strcpy(tempString, bannerMsg);

	if ((monster->dex + rand()%10) > (player->dex + rand()%10))
	{
		damage = (rand() % (monster->str)) + 1 - player->armor->attrib1;
        if (damage > 0)
        {
		    player->hp -= damage;
        }//if
        
        if (damage <= 0)
        {
		    snprintf(bannerMsg, 80, "%s Player absorbed damage!", tempString);
        }//if
		if (player->hp <= 0)
		{
			snprintf(bannerMsg, 80, "%s Killed player!", tempString);
		}//if
		else
			snprintf(bannerMsg, 80, "%s Hit player for %d damage.", tempString, damage);
	}//if
	else
		snprintf(bannerMsg, 80, "%s Missed player.", tempString);
}//monsterAttack

void levelUp(Player *player)
{
    int statChoice;

    if (player->exp > (player->level)*60+(rand()%7+1))
    {
        player->level += 1;
        player->hp += player->level*rand()%8+10;
        statChoice = rand()%3; 
        if (statChoice == 0)
            player->str += 1;
        else if (statChoice == 1)
            player->intell += 1;
        else
            player->dex += 1;
        snprintf(bannerMsg, 80, "Welcome to level %d!", player->level);
    }//if
}//levelUp

void displayInventory(Player *player, ItemDesc* allItems)
{
	int i;
	clear();
	for (i = 0; i < player->itemCount; i++)
	{
        mvprintw(i, 0, "%d. ", i+1);
		mvprintw(i, 4, allItems[player->items[i]].desc);
	}//for
	refresh();
	getch();
}//displayInventory

void useItem(Player *player, ItemType type, ItemDesc* allItems)
{
	int i, itemCount = 0, items[MAXINVENTORY], gotInput = 0, used = 1;
	char c;
	char* typeString;
	clear();

	switch(type)
	{
		case POTION:
			typeString = "Potions:";	
			break;
		case SCROLL:
			typeString = "Scrolls:";	
			break;
        case ANY:
            typeString = "All Items:";
			break;
		default:
			break;
	}//switch
	mvprintw(0, 0, typeString);

	for (i = 0; i < player->itemCount; i++)
	{
        if (type == ANY || allItems[player->items[i]].type == type)
		{
            mvprintw(itemCount+1, 0, "%d. ", itemCount+1);
			mvprintw(itemCount+1, 4, allItems[player->items[i]].desc);
			items[itemCount] = player->items[i];
			itemCount++;
		}//if
	}//for

	refresh();

	while (gotInput == 0 && (c = getch()) != '\n')
	{
		if (c-48 > 0 && c-48 <= itemCount)
		{
			switch(items[c-49])
			{
                case 2:
			        //snprintf(bannerMsg, 80, "Party time!");
                    break;
                case 3:
			        //snprintf(bannerMsg, 80, "I'm scared, Mommy...");
                    break;
				case 4:
					if (rand()%2 == 0)
						player->weapon->attrib1 += 1;
					else
						player->weapon->attrib2 += 1;
					snprintf(player->weapon->desc, 80, "+%d, +%d sword", player->weapon->attrib1, player->weapon->attrib2);
					break;
                //Increase stats for str, dex, int potions
				case 6:
				case 8:
                case 10:
					player->str += allItems[items[c-49]].attrib1;
                    player->intell += allItems[items[c-49]].attrib2;
					player->dex += allItems[items[c-49]].attrib3;
					break;
				case 7:
					player->hp += rand()%8+2;
					break;
                case 9:
                    player->armor->attrib1 += 1;
					snprintf(player->armor->desc, 80, "+%d leather armor", player->armor->attrib1);
                    break;
		        default:
                    used = 0;
			        break;
			}//switch

            if (used)
            {
			    snprintf(bannerMsg, 80, "Used %s!", allItems[items[c-49]].desc);
			    removePlayerItem(player, items[c-49]);
            }//if
            else
            {
			    snprintf(bannerMsg, 80, "Can't use %s.", allItems[items[c-49]].desc);
            }//else
			gotInput = 1;
		}//if
	}//while
}//useItem

void removePlayerItem(Player *player, int item)
{
	int i, index = -1;

	for (i = 0; i < player->itemCount && index == -1; i++)
	{
		if (player->items[i] == item)
			index = i;
	}//for
	
	for (i = index; i < player->itemCount-1; i++)
	{
		player->items[i] = player->items[i+1];
	}//for

	player->itemCount -= 1;
}//removeItem
/*** END PLAYER FUNCTIONS ***/

/*** ITEM FUNCTIONS ***/
void removeItem(Level *level, int y, int x)
{
	int i, index = -1;

	for (i = 0; i < level->itemCount && index == -1; i++)
	{
		if (level->items[i].y == y && level->items[i].x == x)
			index = i;
	}//for
	
	for (i = index; i < level->itemCount-1; i++)
	{
		level->items[i] = level->items[i+1];
	}//for

	level->itemCount -= 1;
}//removeItem

int getItem(Level *level, int y, int x)
{
	int i;

	for (i = 0; i < level->itemCount; i++)
	{
		if (level->items[i].y == y && level->items[i].x == x)
			return level->items[i].itemIndex;
	}//for

	return -1;
}//getItem

void initItems(ItemDesc* allItems)
{
	allItems[0].attrib1 = 1;
	allItems[0].attrib2 = 0;
	allItems[0].attrib3 = 0;
	allItems[0].type = WEAPON;
	allItems[0].weight = 2;
	allItems[0].symbol = ')';
	snprintf(allItems[0].desc, 80, "+%d, +%d sword", allItems[0].attrib1, allItems[0].attrib2);

	allItems[1].attrib1 = 1;
	allItems[1].attrib2 = 0;
	allItems[1].attrib3 = 0;
	allItems[1].type = ARMOR;
	allItems[1].weight = 5;
	allItems[1].symbol = '[';
	snprintf(allItems[1].desc, 80, "+%d leather armor", allItems[1].attrib1);

	allItems[2].attrib1 = 0;
	allItems[2].attrib2 = 0;
	allItems[2].attrib3 = 0;
	allItems[2].type = SCROLL;
	allItems[2].weight = 1;
	allItems[2].symbol = '?';
	snprintf(allItems[2].desc, 80, "scroll of fun");

	allItems[3].attrib1 = 0;
	allItems[3].attrib2 = 0;
	allItems[3].attrib3 = 0;
	allItems[3].type = SCROLL;
	allItems[3].weight = 1;
	allItems[3].symbol = '?';
	snprintf(allItems[3].desc, 80, "scroll of fear");

	allItems[4].attrib1 = 0;
	allItems[4].attrib2 = 0;
	allItems[4].attrib3 = 0;
	allItems[4].type = SCROLL;
	allItems[4].weight = 1;
	allItems[4].symbol = '?';
	snprintf(allItems[4].desc, 80, "scroll of enchant weapon");

	allItems[5].attrib1 = 2;
	allItems[5].attrib2 = 0;
	allItems[5].attrib3 = 0;
	allItems[5].type = MISSILE;
	allItems[5].weight = 2;
	allItems[5].symbol = 'o';
	snprintf(allItems[5].desc, 80, "+%d rock", allItems[5].attrib1);

	allItems[6].attrib1 = 1;
	allItems[6].attrib2 = 0;
	allItems[6].attrib3 = 0;
	allItems[6].type = POTION;
	allItems[6].weight = 1;
	allItems[6].symbol = '!';
	snprintf(allItems[6].desc, 80, "potion of strength");

	allItems[7].attrib1 = 0;
	allItems[7].attrib2 = 0;
	allItems[7].attrib3 = 0;
	allItems[7].type = POTION;
	allItems[7].weight = 1;
	allItems[7].symbol = '!';
	snprintf(allItems[7].desc, 80, "potion of healing");

	allItems[8].attrib1 = 0;
	allItems[8].attrib2 = 0;
	allItems[8].attrib3 = 1;
	allItems[8].type = POTION;
	allItems[8].weight = 1;
	allItems[8].symbol = '!';
	snprintf(allItems[8].desc, 80, "potion of dexterity");

	allItems[9].attrib1 = 0;
	allItems[9].attrib2 = 0;
	allItems[9].attrib3 = 0;
	allItems[9].type = SCROLL;
	allItems[9].weight = 1;
	allItems[9].symbol = '?';
	snprintf(allItems[9].desc, 80, "scroll of enchant armor");

	allItems[10].attrib1 = 0;
	allItems[10].attrib2 = 1;
	allItems[10].attrib3 = 0;
	allItems[10].type = POTION;
	allItems[10].weight = 1;
	allItems[10].symbol = '!';
	snprintf(allItems[10].desc, 80, "potion of intelligence");
}//initItems
/*** END ITEM FUNCTIONS ***/

/*** LEVEL FUNCTIONS ***/
void initLevel(Level *level)
{
	int i=0, j=0;

	for (i = 0; i < LEVELHEIGHT; i++)
		for (j = 0; j < LEVELWIDTH; j++)
        {
			//level->layout[i][j] = '#';
            //level->map[i][j] = ' ';
            updateTile(&level->layout[i][j], '#', '#', 0, 0, 0);
        }//for

	level->itemCount = 0;	
	level->monsterCount = 0;	
}//initLevel

void drawLevel(Level *level)
{
	int i, j;

	for (i = 0; i < LEVELHEIGHT; i++)
		for (j = 0; j < LEVELWIDTH; j++)
        {
            if (level->layout[i][j].visible)
            {
                switch(level->layout[i][j].currentSymbol)
                {
                    case '_':
			            mvaddch(i+2, j+1, ACS_HLINE);
                        break;
                    case '|':
			            mvaddch(i+2, j+1, ACS_VLINE);
                        break;
                    case '{':
			            mvaddch(i+2, j+1, ACS_ULCORNER);
                        break;
                    case '}':
			            mvaddch(i+2, j+1, ACS_URCORNER);
                        break;
                    case '[':
			            mvaddch(i+2, j+1, ACS_LLCORNER);
                        break;
                    case ']':
			            mvaddch(i+2, j+1, ACS_LRCORNER);
                        break;
                    case ':':
			            mvaddch(i+2, j+1, ACS_BLOCK);
                        break;
                    default:
                        mvaddch(i+2, j+1, level->layout[i][j].currentSymbol);
                        break;
                }//switch
            }//if
        }//for
}//drawLevel

void drawLOS(Level *level, Player *player)
{
    int i, j, xstart, xend;
    int newY, newX;
    //int rely = player->y-2, relx = player->x-1;

    /*for (i = 1; i <= player->intell; i++)
    {
        for (j = -1*i; j <= i; j++)
        {
            if (rely-i >= 0 && rely-i < LEVELHEIGHT && relx+j >= 0 && relx+j < LEVELWIDTH &&
                        level->map[rely-i+1][relx+j] != '#')
                level->map[rely-i][relx+j] = level->layout[rely-i][relx+j];
            if (rely+i >= 0 && rely+i < LEVELHEIGHT && relx+j >= 0 && relx+j < LEVELWIDTH &&
                        level->map[rely+i-1][relx+j] != '#')
                level->map[rely+i][relx+j] = level->layout[rely+i][relx+j];
            if (rely+j >= 0 && rely+j < LEVELHEIGHT && relx-i >= 0 && relx-i < LEVELWIDTH &&
                        level->map[rely+j][relx-i+1] != '#')
                level->map[rely+j][relx-i] = level->layout[rely+j][relx-i];
            if (rely+j >= 0 && rely+j < LEVELHEIGHT && relx+i >= 0 && relx+i < LEVELWIDTH &&
                        level->map[rely+j][relx+i-1] != '#')
                level->map[rely+j][relx+i] = level->layout[rely+j][relx+i];
        }//for
    }//for*/

	for (i = 0; i < player->intell; i++)
    {
		for (j = 0; j < player->intell; j++)
        { 
            newY = player->y-(player->intell/2)+i;
            newX = player->x-(player->intell/2)+j;
            if (newY-2 < LEVELHEIGHT && newY-2 >= 0 && newX-1 < LEVELWIDTH && newX-1 >= 0
                    && level->layout[newY-2][newX-1].baseSymbol != '#')
                updateTile(&level->layout[newY-2][newX-1], -1, -1, -1, 1, -1);
        }//for
    }//for
}//drawLOS
/*** END LEVEL FUNCTIONS ***/

/*** MONSTER FUNCTIONS ***/
int getMonster(Level *level, int y, int x)
{
	int i;

	for (i = 0; i < level->monsterCount; i++)
	{
		if (level->monsters[i].y == y && level->monsters[i].x == x && level->monsters[i].alive == 1)
			return i;
	}//for

	return -1;
}//getMonster

int monsterMove(Monster *monster, Level *level, Player *player)
{
    int oldy = monster->y, oldx = monster->x, dy = 0, dx = 0;

    if (rand()%2 == 0)
        dy = 1; 
    else
        dy = -1; 
    if (rand()%2 == 0)
        dx = 1; 
    else
        dx = -1; 

    if (monster->y+dy+2 == player->y && monster->x+dx+1 == player->x)
        monsterAttack(monster, player);
    else if (oldy+dy >= 0 && oldy+dy < LEVELHEIGHT && oldx+dx >= 0 && oldx+dx < LEVELWIDTH &&
                level->layout[oldy+dy][oldx+dx].passable && !level->layout[oldy+dy][oldx+dx].occupied)
                /*level->layout[oldy+dy][oldx+dx] != '-' && level->layout[oldy+dy][oldx+dx] != '|' &&
                level->layout[oldy+dy][oldx+dx] != '{' && level->layout[oldy+dy][oldx+dx] != '}' &&
                level->layout[oldy+dy][oldx+dx] != '[' && level->layout[oldy+dy][oldx+dx] != ']' &&
                level->layout[oldy+dy][oldx+dx] != '#')*/
    {
        monster->y += dy;
        monster->x += dx;

        updateTile(&level->layout[monster->y][monster->x], -1, 'M', -1, -1, -1);
        updateTile(&level->layout[oldy][oldx], -1, '.', -1, -1, -1);
        //level->layout[oldy][oldx] = '.';
    }//if
}//monsterMove
/*** END MONSTER FUNCTIONS ***/

/*** RANDOM GENERATOR FUNCTIONS ***/
void generateLevel(Level* level, Level* prevLevel, int levelNum)
{
	int i, monX, monY, itemX, itemY, items, monsters;
	Room rooms[9], stairRoom;

	initLevel(level);
	level->number = levelNum;

	if (levelNum == 0)
		level->stairsUpRoom = rand()%9;
	else
		level->stairsUpRoom = prevLevel->stairsDownRoom; //levels[levelNum-1].stairsDownRoom;
	do
	{
		level->stairsDownRoom = rand()%9;
	}
	while (level->stairsDownRoom == level->stairsUpRoom);

	for (i = 0; i < 9; i++)
	{
		if (levelNum != 0 && i == level->stairsUpRoom)
		//if (i == level->stairsUpRoom)
		{
			rooms[i].y = 0;
			if (i == 3 || i == 4 || i==5)
				rooms[i].y += (LEVELHEIGHT+1)/3;
			else if (i == 6 || i == 7 || i==8)
				rooms[i].y += ((LEVELHEIGHT+1)/3)*2;
			rooms[i].height = LEVELHEIGHT/3-1;

			rooms[i].x = 0;
			if (i == 1 || i == 4 || i == 7)
				rooms[i].x += LEVELWIDTH/3;
			else if (i == 2 || i == 5 || i == 8)
				rooms[i].x += (LEVELWIDTH/3)*2;
			rooms[i].width = LEVELWIDTH/3-2;
		}//if
		else
		{
			rooms[i].y = rand()%3;
			rooms[i].height = rand()%(LEVELHEIGHT/3-rooms[i].y-2)+2;
			if (i == 3 || i == 4 || i==5)
				rooms[i].y += (LEVELHEIGHT+1)/3;
			else if (i == 6 || i == 7 || i==8)
				rooms[i].y += ((LEVELHEIGHT+1)/3)*2;

			rooms[i].x = rand()%12;
			rooms[i].width = rand()%(LEVELWIDTH/3-rooms[i].x-2)+2;
			if (i == 1 || i == 4 || i == 7)
				rooms[i].x += LEVELWIDTH/3;
			else if (i == 2 || i == 5 || i == 8)
				rooms[i].x += (LEVELWIDTH/3)*2;
		}//else
		makeRoom(level, rooms[i].y, rooms[i].height, rooms[i].x, rooms[i].width);

		if (i == level->stairsUpRoom)
		{
			if (levelNum == 0)
			{
				level->stairsUp.y = rand()%(rooms[i].height-1)+rooms[i].y+1;
				level->stairsUp.x = rand()%(rooms[i].width-1)+rooms[i].x+1;
			}//if
			else
			{
				level->stairsUp.y = prevLevel->stairsDown.y; //levels[levelNum-1].stairsDown.y;
				level->stairsUp.x = prevLevel->stairsDown.x; //levels[levelNum-1].stairsDown.x;
			}//else

            updateTile(&level->layout[level->stairsUp.y][level->stairsUp.x], '<', '<', 1, -1, -1);
			level->layout[level->stairsUp.y][level->stairsUp.x].stairsUp = '1';
		}//if

		if (i == level->stairsDownRoom)
		{
			level->stairsDown.y = rand()%(rooms[i].height-1)+rooms[i].y+1;
			level->stairsDown.x = rand()%(rooms[i].width-1)+rooms[i].x+1;

            updateTile(&level->layout[level->stairsDown.y][level->stairsDown.x], '>', '>', 1, -1, -1);
			level->layout[level->stairsDown.y][level->stairsDown.x].stairsDown = '1';
		}//if

		if (rooms[i].width*rooms[i].height > 9)// && rooms[i].height > 2)
		{
			do
			{
				monY = rand()%(rooms[i].height-1)+rooms[i].y+1;
				monX = rand()%(rooms[i].width-1)+rooms[i].x+1;
			}
			while ((monY == level->stairsUp.y && monX == level->stairsUp.x) || (monY == level->stairsDown.y && monX == level->stairsDown.x));
			addMonster(level, monY, monX);

			do
			{
				itemY = rand()%(rooms[i].height-1)+rooms[i].y+1;
				itemX = rand()%(rooms[i].width-1)+rooms[i].x+1;
			}
			while ((itemY == monY && itemX == monX) || (itemY == level->stairsUp.y && itemX == level->stairsUp.x) || (itemY == level->stairsDown.y && itemX == level->stairsDown.x));
			addItem(level, (rand() % GENITEMS) + NONGENITEMS, itemY, itemX);
		}//if
	}//for

	connectRooms(&rooms[0], &rooms[1], LEFT, level);
	connectRooms(&rooms[1], &rooms[2], LEFT, level);
	connectRooms(&rooms[6], &rooms[7], LEFT, level);
	connectRooms(&rooms[7], &rooms[8], LEFT, level);

	connectRooms(&rooms[0], &rooms[3], DOWN, level);
	connectRooms(&rooms[3], &rooms[6], DOWN, level);
	connectRooms(&rooms[2], &rooms[5], DOWN, level);
	connectRooms(&rooms[5], &rooms[8], DOWN, level);

	connectRooms(&rooms[1], &rooms[4], DOWN, level);
}//generateLevel

void connectRooms(Room *room1, Room *room2, Direction dir, Level *level)
{
	int i = 0;
	int y1, y2, lowY, highY, x1, x2, lowX, highX, yDist, xDist;

	if (dir == LEFT)
	{
		y1 = rand()%(room1->height-1)+room1->y+1;
		x1 = room1->x+room1->width;
		y2 = rand()%(room2->height-1)+room2->y+1;
		x2 = room2->x;

		if (y1 < y2)
		{
			yDist = y2 - y1;
			lowY = y1;
			highY = y2;
		}//if
		else
		{
			yDist = y1 - y2;
			lowY = y2;
			highY = y1;
		}//else
		xDist = x2 - x1;

		for (i = 0; i <= xDist/2; i++)
		{
            updateTile(&level->layout[y1][x1+i], ':', ':', 1, -1, -1);
            updateTile(&level->layout[y2][x2-i], ':', ':', 1, -1, -1);
		}//for
		
		if (yDist == 0)
		{
            updateTile(&level->layout[y1][x1+(xDist/2)+1], ':', ':', 1, -1, -1);
            updateTile(&level->layout[y2][x2+(xDist/2)], ':', ':', 1, -1, -1);
		}//if
		else
		{
			updateTile(&level->layout[lowY][x1+(xDist/2)], ':', ':', 1, -1, -1);

			for (i = 1; i < yDist; i++)
			{
				updateTile(&level->layout[lowY+i][x1+(xDist/2)], ':', ':', 1, -1, -1);
			}//for

			updateTile(&level->layout[highY][x1+(xDist/2)], ':', ':', 1, -1, -1);
		}//else
	}//if
	else if (dir == DOWN)
	{
		x1 = rand()%(room1->width-1)+room1->x+1;
		y1 = room1->y+room1->height;
		x2 = rand()%(room2->width-1)+room2->x+1;
		y2 = room2->y;

		if (x1 < x2)
		{
			xDist = x2 - x1;
			lowX = x1;
			highX = x2;
		}//if
		else
		{
			xDist = x1 - x2;
			lowX = x2;
			highX = x1;
		}//else
		yDist = y2 - y1;

		for (i = 0; i <= yDist/2; i++)
		{
			updateTile(&level->layout[y1+i][x1], ':', ':', 1, -1, -1);
			updateTile(&level->layout[y2-i][x2], ':', ':', 1, -1, -1);
		}//for
		
		if (xDist == 0)
		{
			updateTile(&level->layout[y1+(yDist/2)+1][x1],':',':',1,-1,-1);
			updateTile(&level->layout[y2-(yDist/2)][x2],':',':',1,-1,-1);
		}//if
		else
		{
			updateTile(&level->layout[y1+(yDist/2)][lowX], ':', ':', 1, -1, -1);

			for (i = 1; i < xDist; i++)
			{
				updateTile(&level->layout[y1+(yDist/2)][lowX+i], ':', ':', 1, -1, -1);
			}//for

			updateTile(&level->layout[y1+(yDist/2)][highX], ':', ':', 1, -1, -1);
		}
	}//else if

}//connectRooms

void makeRoom(Level *level, int y, int height, int x, int width)
{
	int i,j;

	for (i = 0; i <= height; i++)
		for (j = 0; j <= width; j++)
        {
            if (i == 0 && j == 0)
			    updateTile(&level->layout[y+i][x+j], '{', '{', 0, -1, -1);
            else if (i == 0 && j == width)
			    updateTile(&level->layout[y+i][x+j], '}', '}', 0, -1, -1);
            else if (i == height && j == 0)
			    updateTile(&level->layout[y+i][x+j], '[', '[', 0, -1, -1);
            else if (i == height && j == width)
			    updateTile(&level->layout[y+i][x+j], ']', ']', 0, -1, -1);
            else if (i == 0 || i == height)
			    updateTile(&level->layout[y+i][x+j], '_', '_', 0, -1, -1);
            else if (j == 0 || j == width)
			    updateTile(&level->layout[y+i][x+j], '|', '|', 0, -1, -1);
            else
			    updateTile(&level->layout[y+i][x+j], '.', '.', 1, -1, -1);
        }//for
}//drawBox

void addMonster(Level *level, int y, int x)
{
	level->monsters[level->monsterCount].str = rand()%3+1 + level->number;
	level->monsters[level->monsterCount].intell = rand()%3+1 + level->number;
	level->monsters[level->monsterCount].dex = rand()%2+1 + level->number;
	level->monsters[level->monsterCount].exp = 7 + level->number;
	level->monsters[level->monsterCount].hp = rand()%4+12 + level->number;
	level->monsters[level->monsterCount].alive = 1;
	level->monsters[level->monsterCount].y = y;
	level->monsters[level->monsterCount].x = x;

	level->monsterCount += 1;
}//addMonster

void addItem(Level *level, int itemIndex, int y, int x)
{
	level->items[level->itemCount].itemIndex = itemIndex;
	level->items[level->itemCount].y = y;
	level->items[level->itemCount].x = x;
	level->itemCount += 1;
}//addItem
/*** END RANDOM GENERATOR FUNCTIONS ***/

/*** TILE FUNCTIONS ***/
void updateTile(Tile *tile, char base, char current, int passable, int visible, int occupied)
{
    if (base != -1)
        tile->baseSymbol = base;
    if (current != -1)
        tile->currentSymbol = current;
    if (passable != -1)
        tile->passable = passable;
    if (visible != -1)
        tile->visible = visible;
    if (occupied != -1)
        tile->occupied = occupied;
}//updateTile
/*** END TILE FUNCTIONS ***/
