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

    private:
        ItemCollection* ic;

};

#endif
