#include "list.h"

List::List()
{
	head = NULL;
	tail = NULL;
    count = 0;
}//List

Node* List::getPosition(int pos)
{
    int i;
    Node* node = NULL;
    
    if (pos >= 0 && pos < count)
    {
        node = head;
        for (i = 0; i < pos; i++)
        {
            node = node->getNext();
        }//for 
    }//if

    return node;
}//getPosition

Node* List::getHead()
{
	return this->head;
}//getHead

void List::setHead(Node* head)
{
	this->head = head;
}//setHead

Node* List::getTail()
{
	return this->tail;
}//getTail

void List::setTail(Node* tail)
{
	this->tail = tail;
}//setHead

void List::push(Node* node)
{
	if (head == NULL)
		head = node;
	if (tail != NULL)
		tail->setNext(node);

	tail = node;
    count++;
}//push

Node* List::pop()
{
	Node* curr = head;
	Node* prev = NULL;

	while (curr != NULL && curr->getNext() != NULL)
	{
		prev = curr;
		curr = curr->getNext();	
	}//while	

	if (prev != NULL)
    {
		prev->setNext(NULL);
    }//if

	tail = prev;
    if (tail == NULL)
        head = tail;

    if (count > 0)
        count--;

	return curr;
}//pop

void List::unshift(Node* node)
{
	node->setNext(head);
	head = node;
	if (head->getNext() == NULL)
		tail = node;
    count++;
}//unshift

Node* List::shift()
{
	Node* node = head;

	if (head != NULL)
    {
		head = head->getNext();
    }//if

	if (head == NULL)
		tail = head;
	
    if (count > 0)
        count--;

	return node;
}//shift

Node* List::remove(int pos)
{
    int i;
    Node* node = NULL;
    Node* prev = NULL;
    
    if (pos >= 0 && pos < count)
    {
        node = head;

        for (i = 0; i < pos; i++)
        {
            prev = node;
            node = node->getNext();
        }//for  
        
        if (prev != NULL)
        {
            prev->setNext(node->getNext());
            if (prev->getNext() == NULL)
                tail = prev;
        }//if
        else
        {
            head = node->getNext();
        }//else

        if (head == NULL)
           tail = head; 
        
        count--;
    }//if

    return node;
}//remove

/*Node* List::search(int value)
{
	Node* curr = head;

	while (curr != NULL && curr->getValue() != value)
	{
		curr = curr->getNext();	
	}//while	

	return curr;
}//search*/

int List::getCount()
{
    return count;
}//getCount

/*void List::printAll()
{
	Node* current = head;
	while (current != NULL)
	{
		cout << current->getValue() << " ";
		current = current->getNext();
	}//while
	cout << endl;
}//printAll*/
