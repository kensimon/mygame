#ifndef __ITEMCOLLECTION_H
#define __ITEMCOLLECTION_H
#include "Item.h"


class ItemCollection
{
public:
		ItemCollection();
		~ItemCollection();
		void push(Item*);
		void pop();
		void drawAll();
		void select(GLdouble x, GLdouble y);
		void removeItem(Item* i);
		Item* getSelected();

private:
		Item* selected;
		Item* head;
		Item* tail;

};

#endif
