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
	void setCollision(Item* item_a, Item* item_b, GLdouble);
	pair<bool, GLdouble> getCollision(Item* item_a, Item* item_b);
	mutex* getCollisionMutex(Item* item_a, Item* item_b);
	boost::shared_mutex* getReadWriteMutex();
	void stopCalculating();
	void startCalculating();
	bool isCalculationStopped();
	int framewait;

private:
	Item* selected;
	list<Item*> items;
	void timerCallback();
	map<pair<Item*, Item*>, pair<bool, GLdouble> > collisions;
	map<pair<Item*, Item*>, mutex* > collision_mutexes;
	mutex getmutex_mutex; //seriously.
	mutex addremove_mutex;
	boost::shared_mutex readwrite_mutex;
	bool phys_stoprequested;
	thread* phys_thread;
};

class collision_iterator
	: public std::iterator<std::forward_iterator_tag, Item*>
{
	friend class ItemCollection;
protected:
	ItemCollection& items;
	Item* base_item;
	std::list<Item*>::iterator items_iterator;

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
		while (
			items_iterator != items.items.end() &&
			(
				*items_iterator == base_item ||
				items.collisions.find(
					std::make_pair(max(base_item, *items_iterator), min(base_item, *items_iterator))
				) != items.collisions.end()
			)
		)
		{
			++items_iterator;
		}
		return *this;
	}
};

#endif
