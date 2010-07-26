#include "ItemCollection.h"
#include "Square.h"
#include "Game.h"
#include <iostream>
#include <cstring>

ItemCollection::ItemCollection(int framewait)
{
	selected = NULL;
	phys_thread = NULL;
	this->framewait = framewait;
	collision_mutexes = (mutex**)malloc(sizeof(mutex*));
	collisions = (CollisionType*)malloc(sizeof(CollisionType));

	collision_mutexes[0] = new mutex();
	collisions[0] = COLL_UNKNOWN;

	collision_bufsize = 1;
}

ItemCollection::~ItemCollection()
{
}

list<Item*>::iterator ItemCollection::end()
{
	return items.end();
}

void ItemCollection::startCalculating()
{
	if (phys_thread == NULL)
	{
		phys_stoprequested = false;
		phys_thread = new thread(boost::bind(&ItemCollection::calculationLoop, this)); //thread will start here.
	}
}

void ItemCollection::stopCalculating()
{
	if (phys_thread != NULL)
	{
		phys_stoprequested = true;
		if (phys_thread->joinable())
		{
			phys_thread->join();
		}
		delete phys_thread;
		phys_thread = NULL;
	}
}

bool ItemCollection::isCalculationStopped()
{
	return (phys_thread == NULL);
}

void ItemCollection::push(Item* n)
{
	mutex::scoped_lock lock(addremove_mutex);
	scoped_write_lock wlock(readwrite_mutex);
	items.push_back(n);
	int size = items.size();
	int itemId = n->getItemId();

	if (size * size > collision_bufsize)
	{
		//We don't have enough mutexes and collision scratch space to store the number of items on the screen.  Time to double the space.
		mutex** tmp_collision_mutexes = (mutex**)realloc(collision_mutexes, sizeof(mutex*) * (size * size * 2));
		CollisionType* tmp_collisions = (CollisionType*)realloc(collisions, sizeof(CollisionType) * (size * size * 2));

		if (tmp_collision_mutexes == NULL || tmp_collisions == NULL)
		{
			//Memory reallocation failed.  Don't add the item.
			items.remove(n);
			delete n;
			return;
		}

		collision_mutexes = tmp_collision_mutexes;
		collisions = tmp_collisions;
		collision_bufsize = size * size * 2;

		for (int i = size - 1; i < collision_bufsize; ++i)
		{
			collision_mutexes[i] = new mutex();
		}
	}
	
	redoItemIds();
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

	i->thread_stoprequested = true;
	mutex::scoped_lock lock(addremove_mutex); //make sure no more object threads are waked
	
	//wait until all object threads are asleep, then immediately relese the lock. that way,
	//if the item to be deleted is waiting on a read lock, it will continue and gracefully quit.
	{ scoped_write_lock wlock(readwrite_mutex); }
	
	i->stop();
	items.remove(i);

	redoItemIds();

	delete i;
	if (selected == i)
	{
		selected = NULL;
	}
}

void ItemCollection::redoItemIds()
{
	int j = 0;
	for (list<Item*>::iterator pos = items.begin(); pos != items.end(); ++pos)
	{
		//re-do all the item IDs.
		(*pos)->setItemId(j++);
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

void ItemCollection::calculationLoop()
{
	while (!phys_stoprequested)
	{
		if (items.size() != 0)
		{
			mutex::scoped_lock lock(addremove_mutex); //Make sure we don't wake threads if objects are being added/removed
			scoped_write_lock writelock(readwrite_mutex); //wait until all threads are done.
			
			memset(collisions, COLL_UNKNOWN, items.size() * sizeof(CollisionType));

			//wake up each thread.  Each one will be waiting for the above write lock to free.
			for (list<Item*>::iterator pos = items.begin(); pos != items.end(); ++pos)
			{
				(*pos)->tick();
			}
		} //<-- release write lock, threads will continue.

		boost::this_thread::sleep(boost::posix_time::time_duration(0, 0, 0, framewait));
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
				selected->resetColor();
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
			selected->resetColor();
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

CollisionType ItemCollection::getCollision(Item *item_a, Item *item_b)
{
	int minid = min(item_a->getItemId(), item_b->getItemId());
	int maxid = max(item_a->getItemId(), item_b->getItemId());
	return collisions[(minid * items.size()) + maxid];
}

void ItemCollection::setCollision(Item* item_a, Item* item_b, CollisionType c)
{
	int minid = min(item_a->getItemId(), item_b->getItemId());
	int maxid = max(item_a->getItemId(), item_b->getItemId());
	collisions[(minid * items.size()) + maxid] = c;
}

mutex* ItemCollection::getCollisionMutex(Item *item_a, Item *item_b)
{
	int minid = min(item_a->getItemId(), item_b->getItemId());
	int maxid = max(item_a->getItemId(), item_b->getItemId());
	return collision_mutexes[(minid * items.size()) + maxid];
}

int ItemCollection::getNextItemId()
{
	mutex::scoped_lock lock(addremove_mutex);
	scoped_write_lock wlock(readwrite_mutex);
	return items.size();
}
