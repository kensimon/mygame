#ifndef __ITEM_H
#define __ITEM_H

#include <GL/glut.h>
#include <math.h>
#include "RTree.h"
#define PI 3.1415926535

#define min(x,y) x < y ? x : y
#define max(x,y) x > y ? x : y

class BBox
{
    public:
    BBox()  {}

    BBox(int a_minX, int a_minY, int a_maxX, int a_maxY)
    {
        min[0] = a_minX;
        min[1] = a_minY;

        max[0] = a_maxX;
        max[1] = a_maxY;
    }

    BBox(GLdouble a_minX, GLdouble a_minY, GLdouble a_maxX, GLdouble a_maxY)
    {
        min[0] = (int)a_minX;
        min[1] = (int)a_minY;

        max[0] = (int)a_maxX;
        max[1] = (int)a_maxY;
    }

    int min[2];
    int max[2];
};

class Item
{
    public:
        Item();
        ~Item();
        void moveTo(GLfloat, GLfloat);
        void resize(GLfloat x);
        void setMass(GLfloat newMass);
        GLdouble getx();
        GLdouble gety();
        GLdouble getobjx();
        GLdouble getobjy();
        void rotate();
        virtual void draw();
        GLfloat getSpin();
        Item* next;
        Item* previous;
        GLfloat getSize();
        void setColor(GLfloat red, GLfloat green, GLfloat blue);
        int spinMomentum;
        GLfloat getMomentumX();
        GLfloat getMomentumY();
        GLfloat getMass();
        void setClickPos(GLdouble, GLdouble);
        void dragTo(GLdouble, GLdouble);
        BBox* getBBox();
        void updateBBox();
        void drawBBox();

    protected:
        GLdouble x;
        GLdouble y;
        GLdouble objx;
        GLdouble objy;
        GLdouble red;
        GLdouble blue;
        GLdouble green;
        GLfloat spin;
        GLfloat size;
        GLfloat momentumX;
        GLfloat momentumY;
        GLfloat mass;
        GLdouble xclickpos;
        GLdouble yclickpos;
        BBox* bbox;
};

#endif
