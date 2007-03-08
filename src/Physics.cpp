#include "Physics.h"
#include "Game.h"
#include "Config.h"

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
        checkBoundaries(cur);
        checkCollisions(cur);
        applyGravity(cur);
        moveWithMomentum(cur);

        cur = cur->next;
    }
}

void Physics::checkBoundaries(Item* item)
{
    Game* instance = Game::getInstance();
    GLdouble xmin = item->getBBox()->min[0];
    GLdouble xmax = item->getBBox()->max[0];

    GLdouble ymin = item->getBBox()->min[1];
    GLdouble ymax = item->getBBox()->max[1];

    if (xmin < 0) {
        item->moveTo(item->getx() - xmin, item->gety());
        item->momentumX = fabs(item->momentumX) * ELASTICITY;
    }

    if (xmax > instance->getWidth()) {
        item->moveTo(xmax - (xmax - instance->getWidth()), item->gety());
        item->momentumX = -(fabs(item->momentumX)) * ELASTICITY;
    }

    if (ymin < 0) {
        item->moveTo(item->getx(), item->gety() - ymin);
        item->momentumY = fabs(item->momentumX) * ELASTICITY;
    }

    if (ymax > instance->getHeight()) {
        item->moveTo(item->getx(), ymax - (ymax - instance->getHeight()));
        item->momentumY = -(fabs(item->momentumY));
    }
}

void Physics::checkCollisions(Item* item)
{
}

void Physics::moveWithMomentum(Item* item)
{
    if (item->grabbed)
        return;
    item->moveTo(item->getx() + item->momentumX, item->gety() + item->momentumY);
    item->momentumX *= VISCOCITY;
    item->momentumY *= VISCOCITY;
}

void Physics::applyGravity(Item* item)
{
    if (item->grabbed)
        return;
    item->momentumY += GRAVITY;
}
