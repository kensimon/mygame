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

list<Item*>::iterator ItemCollection::end()
{
	return items.end();
}

void ItemCollection::push(Item* n)
{
	mutex::scoped_lock lock(addremove_mutex);
	scoped_write_lock wlock(readwrite_mutex);
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

	mutex::scoped_lock lock(addremove_mutex);
	i->thread_stoprequested = true;
	{ //just acquire and release the write lock... in case the object thread is waiting on a read lock, 
	  //we need to let it continue so it can gracefully stop.
		scoped_write_lock wlock(readwrite_mutex);
	}
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

boost::shared_mutex* ItemCollection::getReadWriteMutex()
{
	return &readwrite_mutex;
}

void ItemCollection::calculationLoop(int interval, bool* stoprequested)
{
	while (!(*stoprequested))
	{
		{
			mutex::scoped_lock lock(addremove_mutex); //high-contention lock for adding and removing
			scoped_read_lock readlock(readwrite_mutex); //low-contention lock for tick()'ing threads
			for (list<Item*>::iterator pos = items.begin(); pos != items.end(); ++pos)
			{
				(*pos)->tick();
			}
		}
		boost::this_thread::sleep(boost::posix_time::time_duration(0, 0, 0, interval));
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
		//We have to acqure a lock to make sure no threads are still working while we re-arrange the items.
		mutex::scoped_lock lock(addremove_mutex);
		scoped_write_lock wlock(readwrite_mutex);
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
	return collisions[std::make_pair(max(item_a, item_b), min(item_a, item_b))];
}

void ItemCollection::setCollision(Item* item_a, Item* item_b, GLdouble value)
{
	collisions[std::make_pair(max(item_a, item_b), min(item_a, item_b))] = std::make_pair(true, value);
}

mutex* ItemCollection::getCollisionMutex(Item *item_a, Item *item_b)
{
	mutex::scoped_lock lock(getmutex_mutex);
	pair<Item*, Item*> pair = std::make_pair(max(item_a, item_b), min(item_a, item_b));
	if (collision_mutexes[pair] == NULL)
	{
		collision_mutexes[pair] = new mutex();
	}
	return collision_mutexes[pair];
}
