#include "ItemCollection.h"
#include "Square.h"
#include "Game.h"
#include <iostream>

ItemCollection::ItemCollection() :
collisions_mutex()
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
	mutex::scoped_lock lock(iteration_mutex);
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
	mutex::scoped_lock lock(iteration_mutex);
	if (i == NULL)
		return;

	i->stop();
	items.remove(i);
	delete i;
	if (selected == i)
	{
		selected = NULL;
	}
}

void ItemCollection::removeItem(int num)
{
	removeItem(get(num));
}

void ItemCollection::pop()
{
	removeItem(items.size() - 1);
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

void ItemCollection::calculateAll()
{
	for (list<Item*>::iterator pos = items.begin(); pos != items.end(); ++pos)
	{
		(*pos)->tick();
	}
}

void ItemCollection::select(GLdouble x, GLdouble y)
{
	if (items.size() == 0)
		return;

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

pair<bool, GLdouble> ItemCollection::getCollision(Item* item_a, Item* item_b)
{
	mutex::scoped_lock lock(collisions_mutex);
	return collisions[std::make_pair(max(item_a, item_b), min(item_a, item_b))];
}

void ItemCollection::setCollision(Item* item_a, Item* item_b, GLdouble value)
{
	mutex::scoped_lock lock(collisions_mutex);
	collisions[std::make_pair(max(item_a, item_b), min(item_a, item_b))] = std::make_pair(true, value);
}
