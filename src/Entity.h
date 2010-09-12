#pragma once
#ifndef __ITEM_H
#define __ITEM_H

#include <GL/glut.h>
#include <cmath>
#include <boost/thread.hpp>
#include <boost/bind.hpp>

#include "Config.h"
#include "scoped_rw_lock.h"

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


enum EntityType
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

class Entity
{
    public:
        virtual ~Entity();
        void resize(GLdouble x);
        void setMass(GLdouble newMass);
		void stop();
		void pause();
        GLdouble getx();
        GLdouble gety();
        int getSpinMomentum();
        virtual void draw();
        void drawBBox();
        GLdouble getRotation();
        GLdouble getSize();
        void setColor(GLdouble red, GLdouble green, GLdouble blue);
		void resetColor();
        GLdouble momentumX;
        GLdouble momentumY;
        GLdouble getMass();
        void setClickPos(GLdouble, GLdouble);
        void dragTo(GLdouble, GLdouble);
        bool grabbed;
        int spinMomentum;
		void tick();
		void setEntityId(int id);
		int getEntityId();

    protected:
        Entity(int id, EntityType type);
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
		GLdouble orig_red;
		GLdouble orig_blue;
		GLdouble orig_green;
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
		boost::mutex tick_mutex;
		boost::mutex move_mutex;
		void work();
		thread* tick_thread;
		Game* instance;
		EntityType item_type;
		bool thread_stoprequested;
		int itemId;
};

#endif
