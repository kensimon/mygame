#include "Item.h"

Item::Item()
{
    x = 0;
    y = 0;
    spin = 0;
    spinMomentum = 0;
    size = 0;
    next = NULL;
    previous = NULL;
    red = 1.0;
    green = 1.0;
    blue = 1.0;
    momentumX = 0;
    momentumY = 0;

    //Can't think of anywhere better to put them.
    //They should get moved the first time they're drawn.
    objx = 0;
    objy = 0;

    mass = 1;

    xclickpos = 0;
    yclickpos = 0;
}

Item::~Item()
{
}

void Item::moveTo(GLfloat a, GLfloat b)
{
    momentumX = (a - x);
    momentumY = (b - y);
    x = a;
    y = b;
    //printf("I moved to %lf, %lf\n", x, y);
}

void Item::dragTo(GLdouble a, GLdouble b)
{
    x += a - xclickpos;
    y += b - yclickpos;

    xclickpos = a;
    yclickpos = b;
}

void Item::resize (GLfloat x)
{
    //printf("Resizing to %lf\n", x);
    size += x;
}

void Item::setMass(GLfloat newMass)
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

void Item::rotate()
{
    spin += spinMomentum;
    if (spin > 360.0)
        spin -= 360.0;
    else if (spin < -360.0)
        spin += 360.0;
    //glutPostRedisplay();
}

GLfloat Item::getSpin()
{
    return spin;
}

GLfloat Item::getSize()
{
    return size;
}

void Item::setColor(GLfloat red, GLfloat green, GLfloat blue)
{
    this->red = red;
    this->green = green;
    this->blue = blue;
}

GLfloat Item::getMomentumX()
{
    return momentumX * .9;
}

GLfloat Item::getMomentumY()
{
    return momentumY * .9;
}

GLdouble Item::getobjx()
{
    return objx;
}

GLdouble Item::getobjy()
{
    return objy;
}

GLfloat Item::getMass()
{
    return mass;
}

void Item::setClickPos(GLdouble x, GLdouble y)
{
    xclickpos = x;
    yclickpos = y;
}
