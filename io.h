#ifndef IO_H
#define IO_H

#include "labquest.h"
#include <libtcod/libtcod.hpp>

using namespace std;

class IO
{
	private:
		string banner;
		TCOD_key_t input;
		Coord* changeCoord();
		/*void showInventoryScreen(Player*);
		void showWieldScreen(Player*);
		void showActivateScreen(Player*);
		void showCombineScreen(Player*);
		void showDropScreen(Player*, Level*);
		void examineTile(Coord*, Level*);
		void fastMove(Player*, Level*);*/

    public:
        IO();
		void drawBorder();
        void showLevel(Level*);
		void showAgent(Agent*);
		void clearBanner();
		void printBanner();
		void printStats(Player*);
        void close();
		GameState processInput(Player*, Level*);
		void readInput(bool);
		char getInput();
		bool halt();
		void refresh();
};//Output class

#endif
