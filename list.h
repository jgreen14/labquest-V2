#ifndef LIST_H
#define LIST_H

//#include <iostream>
#include "node.h"

using namespace std;

class List
{
	private:
		Node* head;
		Node* tail;
        int count;

	public:
		List();
        Node* getPosition(int);
		Node* getHead();
		void setHead(Node*);
		Node* getTail();
		void setTail(Node*);
		void push(Node*);
		Node* pop();
		void unshift(Node*);
		Node* shift();
		Node* remove(int);
		//Node* search(int);
        int getCount();
		//void printAll();
};//List class

#endif
