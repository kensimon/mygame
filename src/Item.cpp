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
	stop();
}

void Item::stop()
{
	if (tick_thread.joinable())
	{
		thread_stoprequested = true;
		wait_variable.notify_all();
		tick_thread.join();
	}
}

void Item::dragTo(GLdouble a, GLdouble b)
{
	if (thread_stoprequested)
	{
		return;
	}
	mutex::scoped_lock lock(tick_mutex);

	if (x + (a - xclickpos) + (bbox.max_x - x) > instance->getWidth())
	{
		//trying to push it past the right wall
		x = instance->getWidth() - (bbox.max_x - x);
		momentumX = 0;
	}
	else if (x + (a - xclickpos) - (x - bbox.min_x) < 0)
	{
		//trying to push it past the left wall
		x = x - bbox.min_x;
		momentumX = 0;
	}
	else
	{
		x += a - xclickpos;
		momentumX = (a - xclickpos);
		xclickpos = a;
	}

	if (y + (b - yclickpos) + (bbox.max_y - y) > instance->getHeight())
	{
		//trying to push it past the floor
		y = instance->getHeight() - (bbox.max_y - y);
		momentumY = 0;
	}
	else if (y + (b - yclickpos) - (y - bbox.min_y) < 0)
	{
		//trying to push it past the ceiling
		y = y - bbox.min_y;
		momentumY = 0;
	}
	else
	{
		y += b - yclickpos;
		momentumY = (b - yclickpos);		
		yclickpos = b;
	}
	
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
	gluUnProject(bbox.max_x, bbox.max_y, 0, modelMatrix, projMatrix, viewport, &(bbox.max_objx), &(bbox.max_objy), &objz);
	gluUnProject(bbox.min_x, bbox.min_y, 0, modelMatrix, projMatrix, viewport, &(bbox.min_objx), &(bbox.min_objy), &objz);

    glTranslated(objx, -objy, objz);
    glColor4d(red, green, blue, 0.3);
    glRectd(bbox.min_objx - objx, bbox.min_objy - objy, bbox.max_objx - objx, bbox.max_objy - objy);
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

		if (bbox.max_y >= instance->getHeight())
		{
			/* item hit floor -- bounce off floor */

			momentumY = -momentumY;

			if (instance->getGravityOn() && fabs(momentumY) < GRAVITY)
			{
				/* This helps dampen rounding errors that cause balls to
				   bounce forever */
				momentumY = 0;
				y = instance->getHeight() - (bbox.max_y - y);
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
		if (bbox.min_y < 0)
		{
			/* Reverse ball velocity */
			momentumY = -momentumY;

			/* Ball velocity is reduced by the percentage elasticity */
			momentumY *= elasticity;
			momentumX *= elasticity;

			y += momentumY;
		}

		if (bbox.max_x > instance->getWidth())
		{
			/* Hit right wall */
			/* Reverse ball velocity */
			momentumX = -momentumX;

			/* Ball velocity is reduced by the percentage elasticity */
			momentumX *= elasticity;
			momentumY *= floor_friction;

			/* Bounce off the wall */
			x -= bbox.max_x - instance->getWidth();
		}
		else if (bbox.min_x < 0)
		{
			/* Hit left wall */
			/* Reverse ball velocity */
			momentumX = -momentumX;

			/* Ball velocity is reduced by the percentage elasticity */
			momentumX *= elasticity;

			/* Bounce off the wall */
			x += -(bbox.min_x);
		}
		updateBBox();


		/* Slow ball if it is rolling on the floor */

		if (instance->getGravityOn() && fabs(bbox.max_y) >= instance->getHeight() - GRAVITY &&
				momentumY == 0)
				momentumX *= floor_friction;

		//Collishin Detectshun!
		ItemCollection* items = Game::getInstance()->getItemCollection();
		for (collision_iterator pos(*items, this);
			pos != items->getEnd(); ++pos)
		{
			BBox* otherbbox = &((*pos)->bbox);
			if (otherbbox->max_x > bbox.min_x &&
				otherbbox->min_x < bbox.max_x &&
				otherbbox->max_y > bbox.min_y &&
				otherbbox->min_y < bbox.max_y)
			{
				mutex::scoped_lock lock(Game::getInstance()->stdout_mutex);
				std::cout << "COLLIDE!" << std::endl;
			}
		}
	}
}
