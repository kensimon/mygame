#include "Item.h"
#include <iostream>

Item::Item()
{
    x = 0;
    y = 0;
    degrees = 0;
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
    objsizex = 0;
    objsizey = 0;
    objsizez = 0;

    mass = 1;

    xclickpos = 0;
    yclickpos = 0;

    bbox = new BBox(0, 0, 0, 0);

    grabbed = false;
}

Item::~Item()
{
    delete bbox;
}

void Item::moveTo(GLdouble x, GLdouble y)
{
    this->x = x;
    this->y = y;
    this->updateBBox();
    //printf("I moved to %lf, %lf\n", x, y);
}

void Item::dragTo(GLdouble a, GLdouble b)
{
    x += a - xclickpos;
    y += b - yclickpos;

    momentumX = (a - xclickpos);
    momentumY = (b - yclickpos);

    xclickpos = a;
    yclickpos = b;
    this->updateBBox();
}

void Item::resize (GLdouble x)
{
    //printf("Resizing to %lf\n", x);
    size += x;
    this->updateBBox();
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
    GLdouble objz = 0;
    GLdouble modelMatrix[16];
    GLdouble projMatrix[16];
    GLint viewport[4];

    glPushMatrix();
    glLoadIdentity();

    glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
    glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);
    glGetIntegerv(GL_VIEWPORT, viewport);

    gluUnProject(x, y, 0, modelMatrix, projMatrix, viewport, &objx, &objy, &objz);

    /* Move the square to where it belongs */
    glTranslated(objx, -objy, objz);
    glColor4d(red, green, blue, 0.3);
    glRectd(bbox->min[0] - x, bbox->min[1] - y, bbox->max[0] - x, bbox->max[1] - y);
    //glRectd(10,-10, -10, 10);
    glPopMatrix();
} 

void Item::rotate()
{
    degrees += spinMomentum;
    if (degrees > 360.0)
        degrees -= 360.0;
    else if (degrees < -360.0)
        degrees += 360.0;

    this->updateBBox();
    //glutPostRedisplay();
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

GLdouble Item::getobjx()
{
    return objx;
}

GLdouble Item::getobjy()
{
    return objy;
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

BBox* Item::getBBox()
{
    return bbox;
}

void Item::updateBBox()
{
}
