#ifndef __ITEMCOLLECTION_H
#define __ITEMCOLLECTION_H
#include "Item.h"
#include "scoped_rw_lock.h"
#include <list>
#include <map>
#include <iterator>
#include <boost/thread.hpp>

using std::list;
using std::map;
using std::pair;
using boost::mutex;
using boost::thread;

class collision_iterator; //forward declaration

enum CollisionType
{
	COLL_UNKNOWN,
	COLL_TRUE,
	COLL_FALSE
};

class ItemCollection
{
	friend class collision_iterator;
public:
	ItemCollection(int framewait);
	~ItemCollection();
	void push(Item* i);
    Item* get(int num);
	void pop();
	void drawAll();
	void calculationLoop();
	void select(GLdouble x, GLdouble y);
	void removeItem(Item* i);
	void removeItem(int num);
    int length();
	list<Item*>::iterator end();
	Item* getSelected();
	CollisionType getCollision(Item* item_a, Item* item_b);
	void setCollision(Item* item_a, Item* item_b, CollisionType);
	mutex* getCollisionMutex(Item* item_a, Item* item_b);
	boost::shared_mutex* getReadWriteMutex();
	void stopCalculating();
	void startCalculating();
	bool isCalculationStopped();
	int framewait;
	int getNextItemId();

private:
	Item* selected;
	list<Item*> items;
	void timerCallback();

	CollisionType* collisions;
	mutex** collision_mutexes;

	mutex addremove_mutex;
	boost::shared_mutex readwrite_mutex;
	bool phys_stoprequested;
	thread* phys_thread;
	int collision_bufsize;
	void redoItemIds();
};

class collision_iterator
	: public std::iterator<std::forward_iterator_tag, Item*>
{
	friend class ItemCollection;
protected:
	ItemCollection& items;
	Item* base_item;
	std::list<Item*>::iterator items_iterator;
	std::pair<bool, GLdouble> blankpair;

public:

	explicit collision_iterator(ItemCollection& coll, Item* baseItem)
		: items_iterator(items.items.begin()),
		items(coll)
	{
		base_item = baseItem;
		if (*this == baseItem)
			items_iterator++;
	}

	Item* operator* ()
	{
		return (*items_iterator);
	}

	Item* operator-> ()
	{
		return *(*this);
	}

	collision_iterator& operator= (const Item* value)
	{
		//no-op
		return *this;
	}

	bool operator==( const collision_iterator& other)
	{
		return (*(items_iterator) == *(other.items_iterator));
	}
	
	bool operator==( const list<Item*>::iterator& other)
	{
		return (items_iterator == other);
	}

	bool operator==( const Item* other )
	{
		return ((*items_iterator) == other);
	}

	bool operator!=( const collision_iterator& other)
	{
		return !(*this == other);
	}
	
	bool operator!=( const list<Item*>::iterator& other)
	{
		return !(*this == other);
	}

	collision_iterator& operator++ ()
	{
		if (items_iterator == items.items.end())
			return *this;

		++items_iterator;
		while (items_iterator != items.items.end())
		{
			if (*items_iterator == base_item)
			{
				++items_iterator;
				continue;
			}

			if (items.getCollision(base_item, *items_iterator) == COLL_UNKNOWN) //if we haven't calculated a collision for these two,
			{
				return *this; //use this item.
			}
			else
			{
				++items_iterator; //else, move onto the next one.
				continue;
			}
		}
		return *this;
	}
};


#endif
