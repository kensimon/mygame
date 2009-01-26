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

class BBox
{
    public:
    BBox()  {}
	GLdouble min_x;
	GLdouble min_y;
	GLdouble max_x;
	GLdouble max_y;
	GLdouble max_screenx;
	GLdouble max_screeny;
	GLdouble min_screenx;
	GLdouble min_screeny;

    BBox(GLdouble minX, GLdouble minY, GLdouble maxX, GLdouble maxY)
    {
        min_x = min_screenx = minX;
        min_y = min_screeny = minY;
        max_x = max_screenx = maxX;
        max_y = max_screeny = maxY;
    }
};

class Item
{
    public:
        Item();
        virtual ~Item();
        void moveTo(GLdouble x, GLdouble y);
        void resize(GLdouble x);
        void setMass(GLdouble newMass);
        GLdouble getx();
        GLdouble gety();
        GLdouble getobjx();
        GLdouble getobjy();
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
        virtual void updateBBox();
        GLdouble x;
        GLdouble y;
        GLdouble objx;
        GLdouble objy;
		GLdouble objz;
        GLdouble objsizex;
        GLdouble objsizey;
        GLdouble objsizez;
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
};

#endif
