#ifndef __ITEM_H
#define __ITEM_H

#include <GL/glut.h>
#include <math.h>
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
        void moveTo(GLdouble x, GLdouble y);
        void resize(GLdouble x);
        void setMass(GLdouble newMass);
        GLdouble getx();
        GLdouble gety();
        GLdouble getobjx();
        GLdouble getobjy();
        void rotate();
        virtual void draw();
        void drawBBox();
        GLdouble getSpin();
        Item* next;
        Item* previous;
        GLdouble getSize();
        void setColor(GLdouble red, GLdouble green, GLdouble blue);
        int spinMomentum;
        GLdouble momentumX;
        GLdouble momentumY;
        GLdouble getMass();
        void setClickPos(GLdouble, GLdouble);
        void dragTo(GLdouble, GLdouble);
        BBox* getBBox();
        bool grabbed;
        virtual void updateBBox();

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
        GLdouble spin;
        GLdouble size;
        GLdouble mass;
        GLdouble xclickpos;
        GLdouble yclickpos;
        BBox* bbox;
};

#endif
