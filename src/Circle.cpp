#include "Circle.h"
#include "Game.h"

Circle::Circle(int xpos, int ypos, int itemsize)
{
    x = xpos;
    y = ypos;
    size = itemsize;
    degrees = 0;
    objx = 0;
    objy = 0;
    objz = 0;
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
    glPushMatrix();
    glLoadIdentity();

    glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
    glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);
    glGetIntegerv(GL_VIEWPORT, viewport);
    gluUnProject(x, y, 0, modelMatrix, projMatrix, viewport, &objx, &objy, &objz);

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
	GLdouble z;
    bbox.min_x = x - size;
    bbox.min_y = y + size;
    bbox.max_x = x + size;
    bbox.max_y = y - size;

	glPushMatrix();
    glLoadIdentity();

    glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
    glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);
    glGetIntegerv(GL_VIEWPORT, viewport);
    gluUnProject(x, y, 0, modelMatrix, projMatrix, viewport, &objx, &objy, &objz);
	gluProject(objx + size, objy + size, objz, modelMatrix, projMatrix, viewport, &(bbox.max_screenx), &(bbox.max_screeny), &z);
	gluProject(objx - size, objy - size, objz, modelMatrix, projMatrix, viewport, &(bbox.min_screenx), &(bbox.min_screeny), &z);
	glPopMatrix();
}
