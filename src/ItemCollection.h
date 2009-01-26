#ifndef __ITEMCOLLECTION_H
#define __ITEMCOLLECTION_H
#include "Item.h"
#include <list>

using std::list;

class ItemCollection
{
public:
		ItemCollection();
		~ItemCollection();
		void push(Item* i);
        Item* get(int num);
		void pop();
		void drawAll();
		void select(GLdouble x, GLdouble y);
		void removeItem(Item* i);
		void removeItem(int num);
        int length();
		Item* getSelected();
		list<Item*>::iterator getBeginIterator();
		list<Item*>::iterator getEndIterator();

private:
		Item* selected;
		list<Item*> items;
		void timerCallback();
};

#endif
