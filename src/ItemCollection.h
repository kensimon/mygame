#ifndef __ITEMCOLLECTION_H
#define __ITEMCOLLECTION_H
#include "Item.h"


class ItemCollection
{
public:
		ItemCollection();
		~ItemCollection();
		void push(Item* i);
        Item* get(int num);
        //Item** toArray();
		void pop();
		void drawAll();
		void select(GLdouble x, GLdouble y);
		void removeItem(Item* i);
		void removeItem(int num);
        int length();
		Item* getSelected();

private:
		Item* selected;
		Item* head;
		Item* tail;
        int _length;

};

#endif
