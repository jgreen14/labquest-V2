#include "level.h"
#include "list.h"
#include "coord.h"
#include "items.h"
#include "tiles.h"
#include "room.h"
#include "creature.h"

/*** LEVEL CLASS ***/
Level::Level(int number) : Node(number)
{
	int i, j;
	
	for (i = 0; i < LEVELHEIGHT; i++)
		for (j = 0; j < LEVELWIDTH; j++)
			layout[i][j] = new Tile(BEDROCK);
	
	this->levelNum = number;
    this->upStairs = NULL;
}//Level

string Level::getTileInfo(Coord* position)
{
	string info = "";
    Item* item = NULL;

	switch (layout[position->getY()][position->getX()]->getType())
	{
		case VWALL:
		case HWALL:
		case ULCORNER:
		case URCORNER:
		case LLCORNER:
		case LRCORNER:
			info += "A wall.";
			break;
		case FLOOR:
			info += "The floor.";
			break;
		case BEDROCK:
			info += "Impenetrable rock.";
			break;
		case CORRIDOR:
			info += "A rough corridor.";
			break;
		case WATER:
			info += "Clear, calm water.";
			break;
		case ACID:
			info += "Bubbling, smelly acid.";
			break;
        case UPSTAIRS:
            info += "A creaky, old staircase going up.";
            break;
        case DOWNSTAIRS:
            info += "Creaky, old stairs going further down into the abyss.";
            break;
		default:
			info += "The abyss stares back at you...";
			break;
	}//switch

    //Get names for all items in the tile
    item = layout[position->getY()][position->getX()]->getFirstItem(); 
	while (item != NULL)
    {
		info += "\n" + item->getName();
        item = (Item*)item->getNext();
    }//while

    return info;
}//getTileInfo

/*** RANDOM GENERATOR FUNCTIONS ***/
//void generateLevel(Level* level, Level* prevLevel, int levelNum)
void Level::generateLevel(Level* prevLevel)
{
    int i, y, x, width, height, stairsUpRoom, stairsDownRoom; //, monX, monY, itemX, itemY, items, monsters;
    const int numRooms = 9;
	Room* rooms[numRooms];

	if (levelNum == 0)
		stairsUpRoom = rand()%numRooms;
	else
    	stairsUpRoom = 0;

	do
	{
		stairsDownRoom = rand()%8+1;
	}
	while (stairsDownRoom == stairsUpRoom);

	for (i = 0; i < numRooms; i++)
	{
        //Make the stairsUpRoom the max size so stairs are guaranteed to be in open space
		if (i == stairsUpRoom)
		{
			y = 0;
			if (i == 3 || i == 4 || i==5)
				y += (LEVELHEIGHT+1)/3;
			else if (i == 6 || i == 7 || i==8)
				y += ((LEVELHEIGHT+1)/3)*2;
			height = LEVELHEIGHT/3-1;

			x = 0;
			if (i == 1 || i == 4 || i == 7)
				x += LEVELWIDTH/3;
			else if (i == 2 || i == 5 || i == 8)
				x += (LEVELWIDTH/3)*2;
			width = LEVELWIDTH/3-2;
		}//if
        //Make other rooms a random size
		else
		{
            y = rand()%3;
            height = rand()%(LEVELHEIGHT/3-y-2)+2;
            if (i == 3 || i == 4 || i==5)
                y += (LEVELHEIGHT+1)/3;
            else if (i == 6 || i == 7 || i==8)
                y += ((LEVELHEIGHT+1)/3)*2;

            x = rand()%12;
            width = rand()%(LEVELWIDTH/3-x-2)+2;
            if (i == 1 || i == 4 || i == 7)
            x += LEVELWIDTH/3;
            else if (i == 2 || i == 5 || i == 8)
                x += (LEVELWIDTH/3)*2;
		}//else
        
        rooms[i] = new Room(y, x, width, height);
		makeRoom(rooms[i]->getY(), rooms[i]->getX(), rooms[i]->getWidth(), rooms[i]->getHeight());

        //Get Coord for stairs in stairsUpRoom on level 0 (upStairs is already set to downStairs from prevLevel if level > 0)
		if (levelNum == 0 && i == stairsUpRoom)
		{
            upStairs = new Coord(rand()%(rooms[i]->getHeight()-1)+rooms[i]->getY()+1,
                                    rand()%(rooms[i]->getWidth()-1)+rooms[i]->getX()+1);

            layout[upStairs->getY()][upStairs->getX()]->setType(UPSTAIRS);
		}//if
        else if (i == stairsUpRoom)
        {
            upStairs = prevLevel->getDownStairsCoord();

            layout[upStairs->getY()][upStairs->getX()]->setType(UPSTAIRS);
        }//else if

        //Set Coord for stairs in stairsDownRoom
		if (i == stairsDownRoom)
		{
            downStairs = new Coord(rand()%(rooms[i]->getHeight()-1)+rooms[i]->getY()+1,
                                    rand()%(rooms[i]->getWidth()-1)+rooms[i]->getX()+1);

            layout[downStairs->getY()][downStairs->getX()]->setType(DOWNSTAIRS);
		}//if*/

        //Add monsters and items to rooms
		/*if (rooms[i].width*rooms[i].height > 9)// && rooms[i].height > 2)
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
		}//if*/
	}//for

	connectRooms(rooms[0], rooms[1], LEFT);
	connectRooms(rooms[1], rooms[2], LEFT);
	connectRooms(rooms[6], rooms[7], LEFT);
	connectRooms(rooms[7], rooms[8], LEFT);

	connectRooms(rooms[0], rooms[3], DOWN);
	connectRooms(rooms[3], rooms[6], DOWN);
	connectRooms(rooms[2], rooms[5], DOWN);
	connectRooms(rooms[5], rooms[8], DOWN);

	connectRooms(rooms[1], rooms[4], DOWN);
}//generateLevel

void Level::connectRooms(Room *room1, Room *room2, Direction dir)
{
	int i = 0;
	int y1, y2, lowY, highY, x1, x2, lowX, highX, yDist, xDist;

	if (dir == LEFT)
	{
		y1 = rand()%(room1->getHeight()-1)+room1->getY()+1;
		x1 = room1->getX()+room1->getWidth();
		y2 = rand()%(room2->getHeight()-1)+room2->getY()+1;
		x2 = room2->getX();

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
            //updateTile(&level->layout[y1][x1+i], ':', ':', 1, -1, -1);
            layout[y1][x1+i]->setType(CORRIDOR);
            layout[y1][x1+i]->setPassable(true);

            layout[y2][x2-i]->setType(CORRIDOR);
            layout[y2][x2-i]->setPassable(true);
		}//for
		
		if (yDist == 0)
		{
            layout[y1][x1+(xDist/2)+1]->setType(CORRIDOR);
            layout[y1][x1+(xDist/2)+1]->setPassable(true);

            layout[y2][x2+(xDist/2)]->setType(CORRIDOR);
            layout[y2][x2+(xDist/2)]->setPassable(true);
		}//if
		else
		{
			layout[lowY][x1+(xDist/2)]->setType(CORRIDOR);
			layout[lowY][x1+(xDist/2)]->setPassable(true);

			for (i = 1; i < yDist; i++)
			{
				layout[lowY+i][x1+(xDist/2)]->setType(CORRIDOR);
				layout[lowY+i][x1+(xDist/2)]->setPassable(true);
			}//for

			layout[highY][x1+(xDist/2)]->setType(CORRIDOR);
			layout[highY][x1+(xDist/2)]->setPassable(true);
		}//else
	}//if
	else if (dir == DOWN)
	{
		x1 = rand()%(room1->getWidth()-1)+room1->getX()+1;
		y1 = room1->getY()+room1->getHeight();
		x2 = rand()%(room2->getWidth()-1)+room2->getX()+1;
		y2 = room2->getY();

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
			layout[y1+i][x1]->setType(CORRIDOR);
			layout[y1+i][x1]->setPassable(true);

			layout[y2-i][x2]->setType(CORRIDOR);
			layout[y2-i][x2]->setPassable(true);
		}//for
		
		if (xDist == 0)
		{
			layout[y1+(yDist/2)+1][x1]->setType(CORRIDOR);
			layout[y1+(yDist/2)+1][x1]->setPassable(true);

			layout[y2-(yDist/2)][x2]->setType(CORRIDOR);
			layout[y2-(yDist/2)][x2]->setPassable(true);
		}//if
		else
		{
			layout[y1+(yDist/2)][lowX]->setType(CORRIDOR);
			layout[y1+(yDist/2)][lowX]->setPassable(true);

			for (i = 1; i < xDist; i++)
			{
				layout[y1+(yDist/2)][lowX+i]->setType(CORRIDOR);
				layout[y1+(yDist/2)][lowX+i]->setPassable(true);
			}//for

			layout[y1+(yDist/2)][highX]->setType(CORRIDOR);
			layout[y1+(yDist/2)][highX]->setPassable(true);
		}
	}//else if

}//connectRooms

void Level::makeRoom(int y, int x, int width, int height)
{
	int i,j;

	for (i = 0; i <= height; i++)
		for (j = 0; j <= width; j++)
        {
            if (i == 0 && j == 0)
			    layout[y+i][x+j]->setType(ULCORNER);
            else if (i == 0 && j == width)
			    layout[y+i][x+j]->setType(URCORNER);
            else if (i == height && j == 0)
			    layout[y+i][x+j]->setType(LLCORNER);
            else if (i == height && j == width)
			    layout[y+i][x+j]->setType(LRCORNER);
            else if (i == 0 || i == height)
			    layout[y+i][x+j]->setType(HWALL);
            else if (j == 0 || j == width)
			    layout[y+i][x+j]->setType(VWALL);
            else
            {
			    layout[y+i][x+j]->setType(FLOOR);
			    layout[y+i][x+j]->setPassable(true);
            }//else
        }//for
}//makeRoom
/*** END RANDOM GENERATOR FUNCTIONS ***/

bool Level::inBounds(int y, int x)
{
	if (y >= 0 && y < LEVELHEIGHT && x >= 0 && x < LEVELWIDTH)
		return true;

	return false;
}//inBounds

bool Level::isPassable(int y, int x)
{
	return layout[y][x]->getPassable();
}//isPassable

bool Level::isOccupied(int y, int x)
{
	return layout[y][x]->getOccupied();
}//isOccupied

bool Level::isMapped(int y, int x)
{
	return layout[y][x]->getMapped();
}//isMapped

bool Level::isVisible(int y, int x)
{
	return layout[y][x]->getVisible();
}//isVisible

void Level::map(int y, int x)
{
	layout[y][x]->setMapped(true);
}//map

void Level::visible(int y, int x)
{
	layout[y][x]->setVisible(true);
}//visible

void Level::notVisible(int y, int x)
{
	layout[y][x]->setVisible(false);
}//notVisible

bool Level::addItem(Coord* position, Item* item)
{
	this->layout[position->getY()][position->getX()]->addItem(item);

	//return false;
	return true; //Would this ever return false? Player should always be able to drop item
}//addItem

Item* Level::itemPickup(Coord* position)
{
    return this->layout[position->getY()][position->getX()]->takeItem();
}//itemPickup

bool Level::addCreature(Creature* creature)
{
	if (this->layout[creature->getPosition()->getY()][creature->getPosition()->getX()]->getCreature() == NULL)
	{
		this->layout[creature->getPosition()->getY()][creature->getPosition()->getX()]->setCreature(creature);
		this->layout[creature->getPosition()->getY()][creature->getPosition()->getX()]->setOccupied(true);

		return true;
	}//if

	return false;
}//addCreature

bool Level::removeCreature(Creature* creature)
{
	if (this->layout[creature->getPosition()->getY()][creature->getPosition()->getX()]->getCreature() != NULL)
	{
		this->layout[creature->getPosition()->getY()][creature->getPosition()->getX()]->setCreature(NULL);
		this->layout[creature->getPosition()->getY()][creature->getPosition()->getX()]->setOccupied(false);

		return true;
	}//if

	return false;
}//addCreature

TileType Level::getType(int y, int x)
{
	return this->layout[y][x]->getType();
}//getSymbol

Item* Level::getItem(int y, int x)
{
	return this->layout[y][x]->getFirstItem();
}//getItem

Creature* Level::getCreature(int y, int x)
{
	return this->layout[y][x]->getCreature();
}//getCreature

Coord* Level::getUpStairsCoord()
{
    return this->upStairs;
}//getUpStairsCoord

Coord* Level::getDownStairsCoord()
{
    return this->downStairs;
}//getUpStairsCoord
/*** END LEVEL CLASS ***/
