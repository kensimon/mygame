#include "Physics.h"
#include "Item.h"
#include "Game.h"
#include "Config.h"

Physics::Physics(ItemCollection* ic)
{
    this->ic = ic;
    gravityOn = false;
}

Physics::~Physics()
{
}

void Physics::tick()
{
    Item* item = ic->get(0);
    Game* instance = Game::getInstance();

    while (item != NULL)
    {
		item->spinMomentum *= VISCOCITY * 0.97;

		//if (!item->grabbed)
			item->rotate();

        if (item->grabbed)
        {
#if 0
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
#else

			/*
			 * Make it rotate around an axis where the user clicked
			 * (like a pendulum, or something.)
			 */
			GLdouble clickx = item->xclickpos - item->x;
			GLdouble clicky = -(item->yclickpos - item->y);
			GLdouble len = sqrt(pow(clickx, 2) + pow(clicky, 2));
			GLdouble radians = (item->degrees * (2.0 * PI)) / 360.0;

			GLdouble theta;
			if (clicky >= 0 && clickx >= 0)
				theta = atan(fabs(clicky) / fabs(clickx));
			else if (clicky >= 0 && clickx <= 0)
				theta = (PI / 2) + atan(fabs(clickx) / fabs(clicky));
			else if (clicky <= 0 && clickx <= 0)
				theta = PI + atan(fabs(clicky) / fabs(clickx));
			else
				theta = PI + (PI / 2) + atan(fabs(clickx) / fabs(clicky));

			/* calculation done here. */
			theta += sin(theta + (PI * 1.5)) * -(GRAVITY / len);
			/* end calculation. */

			GLdouble newx, newy = 0;
			if (theta >= 0 && theta <= PI / 2) {
				newx = len * cos(theta);
				newy = len * sin(theta);
			}
			else if (theta >= PI / 2 && theta <= PI) {
				newx = -(len * sin(theta - (PI / 2)));
				newy = len * cos(theta - (PI / 2));
			}
			else if (theta >= PI && theta <= PI * 1.5) {
				newx = -(len * cos(theta - PI));
				newy = -(len * sin(theta - PI));
			}
			else {
				newx = len * sin(theta - (PI * 1.5));
				newy = -(len * cos(theta - (PI * 1.5)));
			}

			item->x = item->xclickpos - newx;
			item->y = item->yclickpos + newy;
#endif

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


        item = item->next;
    }
}
