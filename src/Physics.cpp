#include "Physics.h"
#include "Game.h"

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
        //cur->updateBBox();
        Game::getInstance()->getItemCollection()->getRTree()->Search(cur->getBBox()->min, cur->getBBox()->max, Physics::collisionCallback, cur);
        cur = cur->next;
    }
}

bool Physics::collisionCallback(Item* a_data, void* a_context)
{
    if (a_data != (Item*)a_context)
        std::cout << "collide!" << std::endl;
    return true;
}
