#include "Item.h"
#include "Game.h"
#include <iostream>

Item::Item()
: thread_stoprequested(false),
tick_thread(boost::bind(&Item::work, this)),
elasticity(.9),
floor_friction(.99),
bbox(0,0,0,0)
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
	objz = 0;
    objsizex = 0;
    objsizey = 0;
    objsizez = 0;

    mass = 1;

    xclickpos = 0;
    yclickpos = 0;

    grabbed = false;
}

Item::~Item()
{
	thread_stoprequested = true;
	wait_variable.notify_all();
	tick_thread.join();
}

void Item::moveTo(GLdouble xloc, GLdouble yloc)
{
    x = xloc;
    y = yloc;
    updateBBox();
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
    updateBBox();
}

void Item::resize (GLdouble x)
{
    size += x;
    updateBBox();
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
    glPushMatrix();
    glLoadIdentity();

    glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
    glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);
    glGetIntegerv(GL_VIEWPORT, viewport);

    gluUnProject(x, y, 0, modelMatrix, projMatrix, viewport, &objx, &objy, &objz);

    glTranslated(objx, -objy, objz);
    glColor4d(red, green, blue, 0.3);
    glRectd(bbox.min_x - x, bbox.min_y - y, bbox.max_x - x, bbox.max_y - y);
    glPopMatrix();
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
		if (thread_stoprequested) //the destructor also calls notify_all(), so if we're being destructed, quit now.
		{
			return;
		}

		degrees += spinMomentum;
		if (degrees > 360.0)
			degrees -= 360.0;
		else if (degrees < -360.0)
			degrees += 360.0;

		if (grabbed)
		{
			momentumX = 0;
			momentumY = 0;
			if (bbox.max_screeny > instance->getHeight())
				y = instance->getHeight() - (bbox.max_screeny - y);
			if (bbox.max_screenx > instance->getWidth())
				x = instance->getWidth() - (bbox.max_screenx - x);
			if (bbox.min_screeny < 0)
				y = y - bbox.min_screeny;
			if (bbox.min_screenx < 0)
				x = x - bbox.min_screenx;
			continue; //nothing more to do if we're being grabbed.
		}
		/* Calculate new y position */
		/* Gravity adds to velocity */
		if (instance->getGravityOn() && ((y < instance->getHeight()) || (momentumY != 0)))
		{
			momentumY += GRAVITY;
		}


		/* Move vertically based on velocity */
		momentumY *= VISCOCITY;
		y += momentumY;
		
		/* Move horizontally based on velocity */
		momentumX *= VISCOCITY;
		x = x + momentumX;

		updateBBox();

		if (bbox.max_screeny >= instance->getHeight())
		{
			/* item hit floor -- bounce off floor */

			/* Reverse ball velocity, apply momentum, recalculate bounding box. */
			momentumY = -momentumY;

			if (instance->getGravityOn() && fabs(momentumY) < GRAVITY)
			{
				/* This helps dampen rounding errors that cause balls to
				   bounce forever */
				momentumY = 0;
				y = instance->getHeight() - (bbox.max_screeny - y);
			}
			else
			{
				/* Ball velocity is reduced by the percentage elasticity */
				momentumY *= elasticity;
				momentumX *= floor_friction;
			}
			
			y += momentumY;
		}
		else
		if (bbox.min_screeny < 0)
		{
			/* Reverse ball velocity */
			momentumY = -momentumY;

			/* Ball velocity is reduced by the percentage elasticity */
			momentumY *= elasticity;
			momentumX *= elasticity;

			y += momentumY;
		}

		updateBBox();

		if (bbox.max_screenx > instance->getWidth())
		{
			/* Hit right wall */
			/* Reverse ball velocity */
			momentumX = -momentumX;

			/* Ball velocity is reduced by the percentage elasticity */
			momentumX *= elasticity;
			momentumY *= floor_friction;

			/* Bounce off the wall */
			x -= bbox.max_screenx - instance->getWidth();
		}
		else if (bbox.min_screenx < 0)
		{
			/* Hit left wall */
			/* Reverse ball velocity */
			momentumX = -momentumX;

			/* Ball velocity is reduced by the percentage elasticity */
			momentumX *= elasticity;

			/* Bounce off the wall */
			x += -(bbox.min_screenx);
		}
		updateBBox();


		/* Slow ball if it is rolling on the floor */

		if (instance->getGravityOn() && fabs(bbox.max_screeny) >= instance->getHeight() - GRAVITY &&
				momentumY == 0)
				momentumX *= floor_friction;
	}
}
