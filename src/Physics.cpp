#include "Physics.h"

Physics::Physics(ItemCollection* ic)
{
    this->ic = ic;
}

Physics::~Physics()
{
}

void Physics::tick()
{
    Item* cur = ic->get(0);

    while (cur != NULL)
    {
        //cur->moveTo(cur->getx(), cur->gety() + 1);
        cur->rotate();
        cur->updateBBox();
        cur = cur->next;
    }
}
