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
        static bool collisionCallback(Item*, void*);

    protected:

    private:
        ItemCollection* ic;

};

#endif
