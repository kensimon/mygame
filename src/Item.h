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

    BBox(GLdouble a_minX, GLdouble a_minY, GLdouble a_maxX, GLdouble a_maxY)
    {
        min[0] = a_minX;
        min[1] = a_minY;

        max[0] = a_maxX;
        max[1] = a_maxY;
    }

    GLdouble min[2];
    GLdouble max[2];
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
        void drawBBox();
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
        virtual void updateBBox();
};

#endif
