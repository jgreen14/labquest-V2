#ifndef NODE_H
#define NODE_H

#include <iostream>

using namespace std;

class Node
{
	private:
		Node* next;
		int value;

	public:
        Node();
		Node(int);
		Node* getNext();
		void setNext(Node*);
		int getValue();
		void setValue(int);
};//Node class

#endif
