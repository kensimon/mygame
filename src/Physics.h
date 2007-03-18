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
        bool gravityOn;
#if 0
        static bool collisionCallback(Item*, void*);
#endif

    protected:

    private:
        ItemCollection* ic;
};

#endif
