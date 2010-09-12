#ifndef __ITEMCOLLECTION_H
#define __ITEMCOLLECTION_H
#include "Entity.h"
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

enum CollisionType
{
	COLL_UNKNOWN,
	COLL_TRUE,
	COLL_FALSE,
	COLL_CALCULATING
};

class collision_iterator;

class EntityList
{
	friend class collision_iterator;

public:
	EntityList(int framewait);
	~EntityList();

	typedef collision_iterator iterator;

	void setCollision(Entity& item_a, Entity& item_b, CollisionType);
	CollisionType getCollision(Entity& item_a, Entity& item_b);

	void push_back(Entity* e);
	void pop_back();
    Entity* get(int num);
	void drawAll();
	void calculationLoop();
	void select(GLdouble x, GLdouble y);
	void removeEntity(Entity* e);
	void removeEntity(int num);
    int length();
	list<Entity*>::iterator end();
	Entity* getSelected();
	mutex* getCollisionMutex(Entity* item_a, Entity* item_b);
	boost::shared_mutex* getReadWriteMutex();
	void stopCalculating();
	void startCalculating();
	bool isCalculationStopped();
	int framewait;
	int getNextEntityId();

private:
	Entity* selected;
	list<Entity*> entities;
	void timerCallback();

	CollisionType* collisions;

	mutex** collision_mutexes;

	mutex addremove_mutex;
	boost::shared_mutex readwrite_mutex;
	bool phys_stoprequested;
	thread* phys_thread;
	int collision_bufsize;
	void redoEntityIds();
};

class collision_iterator
	: public std::iterator<std::forward_iterator_tag, Entity*>
{
protected:
	EntityList& entities;
	Entity* base_item;
	std::list<Entity*>::iterator entities_iterator;
	std::pair<bool, GLdouble> blankpair;

public:

	explicit collision_iterator(EntityList& coll, Entity* baseEntity)
		: entities_iterator(entities.entities.begin()),
		entities(coll)
	{
		base_item = baseEntity;
		if (*this == baseEntity)
			entities_iterator++;
	}

	Entity* operator* ()
	{
		return (*entities_iterator);
	}

	Entity* operator-> ()
	{
		return *(*this);
	}

	collision_iterator& operator= (const Entity* value)
	{
		//no-op
		return *this;
	}

	bool operator==( const collision_iterator& other)
	{
		return (*(entities_iterator) == *(other.entities_iterator));
	}
	
	bool operator==( const list<Entity*>::iterator& other)
	{
		return (entities_iterator == other);
	}

	bool operator==( const Entity* other )
	{
		return ((*entities_iterator) == other);
	}

	bool operator!=( const collision_iterator& other)
	{
		return !(*this == other);
	}
	
	bool operator!=( const list<Entity*>::iterator& other)
	{
		return !(*this == other);
	}

	collision_iterator& operator++ ()
	{
		if (entities_iterator == entities.entities.end())
			return *this;

		++entities_iterator;
		while (entities_iterator != entities.entities.end())
		{
			if (*entities_iterator == base_item)
			{
				++entities_iterator;
				continue;
			}

			if (entities.getCollision(*base_item, **entities_iterator) == COLL_UNKNOWN) //if we haven't calculated a collision for these two,
			{
				entities.setCollision(*base_item, **entities_iterator, COLL_CALCULATING);
				return *this; //use this item.
			}
			else
			{
				++entities_iterator; //else, move onto the next one.
				continue;
			}
		}
		return *this;
	}
};


#endif
