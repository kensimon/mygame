#include "Physics.h"
#include "Game.h"
#include "Config.h"

Physics::Physics(ItemCollection* ic)
{
    this->ic = ic;
    gravityOn = true;
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

        cur = cur->next;
    }
}

void Physics::checkBoundaries(Item* item)
{
    Game* instance = Game::getInstance();
    if (item->grabbed)
    {
        item->momentumX = 0;
        item->momentumY = 0;
        if (item->gety() > instance->getHeight())
            item->moveTo(item->getx(), instance->getHeight());
        if (item->getx() > instance->getWidth())
            item->moveTo(instance->getWidth(), item->gety());
        if (item->gety() < 0)
            item->moveTo(item->getx(), 0);
        if (item->getx() < 0)
            item->moveTo(0, item->gety());
        return;
    }
    /* Calculate new y position */
    /* Gravity adds to velocity */
    if (gravityOn && ((item->gety() < instance->getHeight()) || (item->momentumY != 0)))
    {
        item->momentumY += GRAVITY;
    }


    /* Move vertically based on velocity */
    item->momentumY *= VISCOCITY;
    item->moveTo(item->getx(), item->gety() + item->momentumY);

    if (item->gety() >= instance->getHeight())
    {
        /* item hit floor -- bounce off floor */

        /* Reverse ball velocity */
        item->momentumY = -item->momentumY;

        item->moveTo(item->getx(), item->gety() + item->momentumY);

        if (gravityOn && fabs(item->momentumY) < GRAVITY)
            /* This helps dampen rounding errors that cause balls to
               bounce forever */
            item->momentumY = 0;
        else
        {
            /* Ball velocity is reduced by the percentage elasticity */
            item->momentumY *= ELASTICITY;
            item->moveTo(item->getx(), instance->getHeight() -
                    (instance->getHeight() - item->gety()) * ELASTICITY);
        }
    }
    else
        if (item->gety() < 0)
        {
            /* Reverse ball velocity */
            item->momentumY = -item->momentumY;

            /* Ball velocity is reduced by the percentage elasticity */
            item->momentumY *= ELASTICITY;

            /* Bounce off the wall */
            item->moveTo(item->getx(), -item->gety());
        }


    /* Calculate new x position */
    /* Move horizontally based on velocity */
    item->momentumX *= VISCOCITY;
    item->moveTo(item->getx() + item->momentumX, item->gety());

    if (item->getx() > instance->getWidth())
    {
        /* Hit right wall */
        /* Reverse ball velocity */
        item->momentumX = -item->momentumX;

        /* Ball velocity is reduced by the percentage elasticity */
        item->momentumX *= ELASTICITY;

        /* Bounce off the wall */
        item->moveTo(-item->getx() + instance->getWidth() * 2, item->gety());
    }
    else if (item->getx() < 0)
    {
        /* Hit left wall */
        /* Reverse ball velocity */
        item->momentumX = -item->momentumX;

        /* Ball velocity is reduced by the percentage elasticity */
        item->momentumX *= ELASTICITY;

        /* Bounce off the wall */
        item->moveTo(-item->getx(), item->gety());
    }


    /* Slow ball if it is rolling on the floor */

    if (gravityOn && fabs(item->gety()) >= instance->getHeight() - GRAVITY &&
            item->momentumY == 0)
        item->momentumX *= ELASTICITY;


}

void Physics::checkCollisions(Item* item)
{
}
