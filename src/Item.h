#pragma once
#ifndef __ITEM_H
#define __ITEM_H

#include <GL/glut.h>
#include <cmath>
#include <boost/thread.hpp>

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
	GLdouble min[2];
    GLdouble max[2];

    BBox(GLdouble a_minX, GLdouble a_minY, GLdouble a_maxX, GLdouble a_maxY)
    {
        min[0] = a_minX;
        min[1] = a_minY;

        max[0] = a_maxX;
        max[1] = a_maxY;
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
        void rotate();
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
        BBox* getBBox();
        bool grabbed;
        virtual void updateBBox();
        int spinMomentum;
		bool thread_stoprequested;
		void tick();
		boost::condition_variable wait_variable;

    protected:
        GLdouble x;
        GLdouble y;
        GLdouble objx;
        GLdouble objy;
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
        BBox* bbox;
		boost::mutex tick_mutex;
		void work();
		thread tick_thread;
		Game* instance;
		bool calculating;
};

#endif
