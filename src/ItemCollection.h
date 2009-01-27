#ifndef __ITEMCOLLECTION_H
#define __ITEMCOLLECTION_H
#include "Item.h"
#include <list>
#include <map>
#include <iterator>
#include <boost/thread.hpp>

using std::list;
using std::map;
using std::pair;
using boost::mutex;

class collision_iterator; //forward declaration

class ItemCollection
{
	friend class collision_iterator;
public:
	ItemCollection();
	~ItemCollection();
	void push(Item* i);
    Item* get(int num);
	void pop();
	void drawAll();
	void calculateAll();
	void select(GLdouble x, GLdouble y);
	void removeItem(Item* i);
	void removeItem(int num);
    int length();
	Item* getSelected();
	void setCollision(Item* item_a, Item* item_b, GLdouble);
	GLdouble getCollision(Item* item_a, Item* item_b);

private:
	Item* selected;
	list<Item*> items;
	void timerCallback();
	map<pair<Item*, Item*>, GLdouble> collisions;
	map<pair<Item*, Item*>, mutex> mutexes;
	mutex collisions_mutex;
	mutex iteration_mutex;
};

class collision_iterator
	: public std::iterator<std::forward_iterator_tag, Item*>
{
	friend class ItemCollection;
protected:
	ItemCollection& items;
	Item* base_item;
	std::list<Item*>::iterator items_iterator;

	explicit collision_iterator(ItemCollection& coll, Item* baseItem)
		: items_iterator(items.items.begin()),
		items(coll)
	{
		base_item = baseItem;
	}

	Item* operator* ()
	{
		return *items_iterator;
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
		return ( items.items == other.items.items );
	}

	bool operator!=( const collision_iterator& other)
	{
		return (!((*this) == other));
	}
};

#endif
