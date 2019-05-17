#ifndef AGENT_H
#define AGENT_H

#include "labquest.h"

using namespace std;

class Agent
{
	private:
		int str;
		int dex;
		int per;
		string name;
		char symbol;

	protected:
		Coord* position;

	public:
		Agent();
		Agent(int, int, int, string, Coord*, char);
		
		int getStr();
		int getDex();
		int getPer();
		string getName();
		Coord* getPosition();
		char getSymbol();
		void setStr(int);
		void setDex(int);
		void setPer(int);
		void setName(string);
		void setPosition(Coord*);
		void setSymbol(char symbol);
};//Agent class

#endif
