#ifndef __ITEM_H
#define __ITEM_H

#include <GL/glut.h>

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

};
#endif
