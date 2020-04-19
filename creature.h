#ifndef CREATURE_H
#define CREATURE_H

#include "labquest.h"
#include "agent.h"

using namespace std;

class Creature : public Agent
{
	private:
		bool hostile;
		Direction direction;
		CreatureType type;

	public:
		Creature();
		Creature(int, int, int, string, Coord*, char);
		Creature(int, int, int, string, Coord*, char, CreatureType);

		bool move(Level*);

		bool getHostile();
		void setHostile(bool);
		CreatureType getType();
		void setType(CreatureType);
};//Creature class

#endif
