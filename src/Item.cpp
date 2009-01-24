#include "Item.h"
#include "Game.h"
#include <iostream>

Item::Item()
: thread_stoprequested(false),
tick_thread(boost::bind(&Item::work, this))
{
	instance = Game::getInstance();
    x = 0;
    y = 0;
    degrees = 0;
    spinMomentum = 0;
    size = 0;
    red = 1.0;
    green = 1.0;
    blue = 1.0;
    momentumX = 0;
    momentumY = 0;

    //Can't think of anywhere better to put them.
    //They should get moved the first time they're drawn.
    objx = 0;
    objy = 0;
    objsizex = 0;
    objsizey = 0;
    objsizez = 0;

    mass = 1;

    xclickpos = 0;
    yclickpos = 0;

    bbox = new BBox(0, 0, 0, 0);
    grabbed = false;
}

Item::~Item()
{
	thread_stoprequested = true;
	wait_variable.notify_all();
	tick_thread.join();
    delete bbox;
}

void Item::moveTo(GLdouble x, GLdouble y)
{
    this->x = x;
    this->y = y;
    this->updateBBox();
}

void Item::dragTo(GLdouble a, GLdouble b)
{
	if (thread_stoprequested)
	{
		return;
	}
	mutex::scoped_lock lock(tick_mutex);
    x += a - xclickpos;
    y += b - yclickpos;

    momentumX = (a - xclickpos);
    momentumY = (b - yclickpos);

    xclickpos = a;
    yclickpos = b;
	tick_thread.detach();
    this->updateBBox();
}

void Item::resize (GLdouble x)
{
    //printf("Resizing to %lf\n", x);
    size += x;
    this->updateBBox();
}

void Item::setMass(GLdouble newMass)
{
    mass = newMass;
}

GLdouble Item::getx()
{
    return x;
}

GLdouble Item::gety()
{
    return y;
}

void Item::draw()
{
} 

void Item::drawBBox()
{
    GLdouble objz = 0;
    GLdouble modelMatrix[16];
    GLdouble projMatrix[16];
    GLint viewport[4];

    glPushMatrix();
    glLoadIdentity();

    glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
    glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);
    glGetIntegerv(GL_VIEWPORT, viewport);

    gluUnProject(x, y, 0, modelMatrix, projMatrix, viewport, &objx, &objy, &objz);

    /* Move the square to where it belongs */
    glTranslated(objx, -objy, objz);
    glColor4d(red, green, blue, 0.3);
    glRectd(bbox->min[0] - x, bbox->min[1] - y, bbox->max[0] - x, bbox->max[1] - y);
    //glRectd(10,-10, -10, 10);
    glPopMatrix();
} 

void Item::rotate()
{
    degrees += spinMomentum;
    if (degrees > 360.0)
        degrees -= 360.0;
    else if (degrees < -360.0)
        degrees += 360.0;

    this->updateBBox();
}

GLdouble Item::getRotation()
{
    return degrees;
}

GLdouble Item::getSize()
{
    return size;
}

void Item::setColor(GLdouble red, GLdouble green, GLdouble blue)
{
    this->red = red;
    this->green = green;
    this->blue = blue;
}

GLdouble Item::getobjx()
{
    return objx;
}

GLdouble Item::getobjy()
{
    return objy;
}

GLdouble Item::getMass()
{
    return mass;
}

void Item::setClickPos(GLdouble x, GLdouble y)
{
    xclickpos = x;
    yclickpos = y;
}

BBox* Item::getBBox()
{
    return bbox;
}

void Item::updateBBox()
{
}

void Item::tick()
{
	wait_variable.notify_all();
}

void Item::work()
{
	while (!thread_stoprequested)
	{
		mutex::scoped_lock lock(tick_mutex);
		wait_variable.wait(lock);
		rotate();

		if (grabbed)
		{
			momentumX = 0;
			momentumY = 0;
			if (gety() > instance->getHeight())
				moveTo(getx(), instance->getHeight());
			if (getx() > instance->getWidth())
				moveTo(instance->getWidth(), gety());
			if (gety() < 0)
				moveTo(getx(), 0);
			if (getx() < 0)
				moveTo(0, gety());
			continue;
		}
		/* Calculate new y position */
		/* Gravity adds to velocity */
		if (instance->getGravityOn() && ((gety() < instance->getHeight()) || (momentumY != 0)))
		{
			momentumY += GRAVITY;
		}


		/* Move vertically based on velocity */
		momentumY *= VISCOCITY;
		moveTo(getx(), gety() + momentumY);

		if (gety() >= instance->getHeight())
		{
			/* item hit floor -- bounce off floor */

			/* Reverse ball velocity */
			momentumY = -momentumY;

			moveTo(getx(), gety() + momentumY);

			if (instance->getGravityOn() && fabs(momentumY) < GRAVITY)
				/* This helps dampen rounding errors that cause balls to
				   bounce forever */
				momentumY = 0;
			else
			{
				/* Ball velocity is reduced by the percentage elasticity */
				momentumY *= ELASTICITY;
				moveTo(getx(), instance->getHeight() -
						(instance->getHeight() - gety()) * ELASTICITY);
			}
		}
		else
			if (gety() < 0)
			{
				/* Reverse ball velocity */
				momentumY = -momentumY;

				/* Ball velocity is reduced by the percentage elasticity */
				momentumY *= ELASTICITY;

				/* Bounce off the wall */
				moveTo(getx(), -gety());
			}


		/* Calculate new x position */
		/* Move horizontally based on velocity */
		momentumX *= VISCOCITY;
		moveTo(getx() + momentumX, gety());

		if (getx() > instance->getWidth())
		{
			/* Hit right wall */
			/* Reverse ball velocity */
			momentumX = -momentumX;

			/* Ball velocity is reduced by the percentage elasticity */
			momentumX *= ELASTICITY;

			/* Bounce off the wall */
			moveTo(-getx() + instance->getWidth() * 2, gety());
		}
		else if (getx() < 0)
		{
			/* Hit left wall */
			/* Reverse ball velocity */
			momentumX = -momentumX;

			/* Ball velocity is reduced by the percentage elasticity */
			momentumX *= ELASTICITY;

			/* Bounce off the wall */
			moveTo(-getx(), gety());
		}


		/* Slow ball if it is rolling on the floor */

		if (instance->getGravityOn() && fabs(gety()) >= instance->getHeight() - GRAVITY &&
				momentumY == 0)
			momentumX *= ELASTICITY;
	}
}
