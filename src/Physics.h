#ifndef __PHYSICS_H
#define __PHYSICS_H
#include "ItemCollection.h"
#include <iostream>

class Physics
{
    public:
        Physics(ItemCollection* ic);
        ~Physics();
        void tick();
#if 0
        static bool collisionCallback(Item*, void*);
#endif

    protected:

    private:
        ItemCollection* ic;
        void checkBoundaries(Item* item);
        void checkCollisions(Item* item);
        void moveWithMomentum(Item* item);
        void applyGravity(Item* item);
};

#endif
