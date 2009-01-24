#include "Circle.h"
#include "Game.h"

Circle::Circle(int xpos, int ypos, int size)
{
    this->x = xpos;
    this->y = ypos;
    this->size = size;
    degrees = 0;
    objx = 0;
    objy = 0;
    objsizex = 0;
    objsizey = 0;
    objsizez = 0;
    quad = gluNewQuadric();
	floor_friction = 0.99;
	elasticity = 0.9;
}

Circle::~Circle()
{
	gluDeleteQuadric(quad);
}

void Circle::draw()
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
    gluUnProject(size, 0, 0, modelMatrix, projMatrix, viewport, &objsizex, &objsizey, &objsizez);

    glTranslated(objx, -objy, objz);
    glRotated(degrees, 0, 0, objz);
    glColor4d(red, green, blue, 0.8);
    gluDisk(quad, 0, size, 30, 1);
    glColor4d(0,0,0, 0.8);
    gluDisk(quad, 0, size*.9, 30, 1);
    glPopMatrix();
} 

void Circle::updateBBox()
{
    bbox.min[0] = x - size;
    bbox.min[1] = y + size;
    bbox.max[0] = x + size;
    bbox.max[1] = y - size;
}
