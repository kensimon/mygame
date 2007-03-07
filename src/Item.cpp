#include "Item.h"
#include <iostream>

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

    bbox = new BBox(0, 0, 0, 0);
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
    this->updateBBox();
    //printf("I moved to %lf, %lf\n", x, y);
}

void Item::dragTo(GLdouble a, GLdouble b)
{
    x += a - xclickpos;
    y += b - yclickpos;

    xclickpos = a;
    yclickpos = b;
    this->updateBBox();
}

void Item::resize (GLfloat x)
{
    //printf("Resizing to %lf\n", x);
    size += x;
    this->updateBBox();
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
    glTranslatef(objx, -objy, objz);
    glColor4f(red, green, blue, 0.3);
    glRectf(bbox->min[0] - x, bbox->min[1] - y, bbox->max[0] - x, bbox->max[1] - y);
    //glRectf(10,-10, -10, 10);
    glPopMatrix();
} 

void Item::rotate()
{
    spin += spinMomentum;
    if (spin > 360.0)
        spin -= 360.0;
    else if (spin < -360.0)
        spin += 360.0;

    this->updateBBox();
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

BBox* Item::getBBox()
{
    return bbox;
}

void Item::updateBBox()
{
}
