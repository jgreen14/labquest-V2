#include "agent.h"
#include "coord.h"

/*** AGENT CLASS***/
Agent::Agent()
{
	this->str = 0;
	this->dex = 0;
	this->per = 0;
	this->name = "";
	this->position = new Coord(0,0);
	this->symbol = ' ';
}//Agent

Agent::Agent(int str, int dex, int per, string name, Coord* position, char symbol)
{
	this->str = str;
	this->dex = dex;
	this->per = per;
	this->name = name;
	this->position = position;
	this->symbol = symbol;
}//Agent

/*** GETTERS/SETTERS ***/
int Agent::getStr()
{
	return this->str;
}//getStr

int Agent::getDex()
{
	return this->dex;
}//getDex

int Agent::getPer()
{
	return this->per;
}//getPer

string Agent::getName()
{
	return this->name;
}//getName

Coord* Agent::getPosition()
{
	return this->position;
}//getPosition

char Agent::getSymbol()
{
	return this->symbol;
}//getSymbol

void Agent::setStr(int str)
{
	this->str = str;
}//setStr

void Agent::setDex(int dex)
{
	this->dex = dex;
}//setDex

void Agent::setPer(int per)
{
	this->per = per;
}//setPer

void Agent::setName(string name)
{
	this->name = name;
}//setName

void Agent::setPosition(Coord* position)
{
	this->position = position;
}//setPosition

void Agent::setSymbol(char symbol)
{
	this->symbol = symbol;
}//setSymbol
/*** END AGENT CLASS***/
