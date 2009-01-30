#pragma once
#ifndef __ITEM_H
#define __ITEM_H

#include <GL/glut.h>
#include <cmath>
#include <boost/thread.hpp>
#include <boost/bind.hpp>

#include "Config.h"

#define PI 3.1415926535

#ifndef min
#define min(x,y) (((x) < (y)) ? (x) : (y))
#endif

#ifndef max
#define max(x,y) (((x) > (y)) ? (x) : (y))
#endif

using boost::thread;
using boost::mutex;

class Game;


enum ItemType
{
	SquareType, CircleType
};

class BBox
{
    public:
    BBox()  {}
	GLdouble min_x;
	GLdouble min_y;
	GLdouble max_x;
	GLdouble max_y;

	GLdouble max_objx;
	GLdouble max_objy;
	GLdouble min_objx;
	GLdouble min_objy;

    BBox(GLdouble minX, GLdouble minY, GLdouble maxX, GLdouble maxY)
    {
        min_x = min_objx = minX;
        min_y = min_objy = minY;
        max_x = max_objx = maxX;
        max_y = max_objy = maxY;
    }
};

class Item
{
    public:
        virtual ~Item();
        void resize(GLdouble x);
        void setMass(GLdouble newMass);
		void stop();
        GLdouble getx();
        GLdouble gety();
        int getSpinMomentum();
        virtual void draw();
        void drawBBox();
        GLdouble getRotation();
        GLdouble getSize();
        void setColor(GLdouble red, GLdouble green, GLdouble blue);
        GLdouble momentumX;
        GLdouble momentumY;
        GLdouble getMass();
        void setClickPos(GLdouble, GLdouble);
        void dragTo(GLdouble, GLdouble);
        bool grabbed;
        int spinMomentum;
		void tick();

    protected:
        Item(ItemType type);
        virtual void updateBBox();
        GLdouble x;
        GLdouble y;
        GLdouble objx;
        GLdouble objy;
		GLdouble objz;
        GLdouble objsizex;
        GLdouble objsizey;
        GLdouble objsizez;
		GLdouble objsize;
        GLdouble red;
        GLdouble blue;
        GLdouble green;
        GLdouble degrees;
        GLdouble size;
        GLdouble mass;
        GLdouble xclickpos;
        GLdouble yclickpos;
		GLdouble elasticity;
		GLdouble floor_friction;
		GLdouble modelMatrix[16];
		GLdouble projMatrix[16];
		GLint viewport[4];
		boost::condition_variable wait_variable;
        BBox bbox;
		bool thread_stoprequested;
		boost::mutex tick_mutex;
		void work();
		thread tick_thread;
		Game* instance;
		ItemType item_type;
};

#endif
