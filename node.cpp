#include "node.h"

Node::Node()
{
	this->value = 0;
	this->next = NULL;
}//Node

Node::Node(int value)
{
	this->value = value;
	this->next = NULL;
}//Node

Node* Node::getNext()
{
	return this->next;
}//getNext
	
void Node::setNext(Node* next)
{
	this->next = next;
}//setNext

int Node::getValue()
{
	return this->value;
}//getValue

void Node::setValue(int value)
{
	this->value = value;
}//setValue
