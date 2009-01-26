#include "ItemCollection.h"
#include "Square.h"
#include "Game.h"
#include <iostream>

ItemCollection::ItemCollection()
{
	selected = NULL;
}


ItemCollection::~ItemCollection()
{
}

void ItemCollection::push(Item* n)
{
	items.push_back(n);
}

Item* ItemCollection::get(int num)
{
    int i = 0;
	for (list<Item*>::iterator pos = items.begin(); pos != items.end(); ++pos)
	{
		if (i++ == num)
		{
			return *pos;
		}
	}
    return NULL;
}

void ItemCollection::removeItem(Item* i)
{
	if (i == NULL)
		return;

	items.remove(i);
	delete i;
	if (selected == i)
	{
		selected = NULL;
	}
}

void ItemCollection::removeItem(int num)
{
	Item* toDelete = get(num);
	removeItem(toDelete);
}

void ItemCollection::pop()
{
	if (items.size() == 0)
		return;

	Item* toDelete = items.back();
	items.pop_back();
	delete toDelete;

	if (selected == toDelete)
	{
		selected = NULL;
	}
}

void ItemCollection::drawAll()
{
    bool dbb = Game::getInstance()->drawBBoxes;
	for (list<Item*>::iterator pos = items.begin(); pos != items.end(); ++pos)
	{
		if (dbb) {
			(*pos)->drawBBox();
		}
		(*pos)->draw();
	}
}

void ItemCollection::select(GLdouble x, GLdouble y)
{
    Item* cur;
    bool done = false;
	
	/* start from the end and work our way back to the beginning to find the 
	 * object.  We do this because the end has the elements that were drawn 
	 * last, and we want to select the item that the user is seeing, not the one
	 * that's drawn underneath.
	 */
	for (list<Item*>::reverse_iterator pos = items.rbegin(); pos != items.rend() && !done; ++pos)
	{
		done = true;
		cur = *(pos);

        // Is this what we tried to click on?
        if (x <= (cur->getx() + cur->getSize()) &&
                x >= (cur->getx() - cur->getSize()) &&
                y <= (cur->gety() + cur->getSize()) &&
                y >= (cur->gety() - cur->getSize()))
        {
			if (selected != NULL) //de-color the previously selected item 
			{
				selected->setColor(1,1,1);
			}
            selected = cur;
        }
        else
        {
            // Nope, try the next one.
            done = false;
			cur = NULL; //In case this is the last one.
        }
    }

    // If we haven't found one yet...
    if (cur == NULL)
    {
        // Select nothing.
		if (selected != NULL)
			selected->setColor(1,1,1);
        selected = NULL;
    }

	//Move selected to the back, so it gets rendered on top.
	if (selected != NULL)
    {
		items.remove(selected);
		items.push_back(selected);
    }

    if (selected != NULL)
        selected->setColor( .8, 0, 0);
}

Item* ItemCollection::getSelected()
{
    return selected;
}

int ItemCollection::length()
{
	return items.size();
}

list<Item*>::iterator ItemCollection::getBeginIterator()
{
	return items.begin();
}

list<Item*>::iterator ItemCollection::getEndIterator()
{
	return items.end();
}
