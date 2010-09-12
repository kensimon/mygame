#include "Circle.h"
#include "Game.h"

Circle::Circle(int id, int xpos, int ypos, int itemsize)
: Entity(id, CircleType)
{
    x = xpos;
    y = ypos;
    size = itemsize;
    degrees = 0;
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
	gluUnProject(x + size, y + size, 0, modelMatrix, projMatrix, viewport, &objsizex, &objsizey, &objsizez);

	objsize = objsizex - objx;

    glTranslated(objx, -objy, objz);
    glRotated(degrees, 0, 0, objz);
    glColor4d(red, green, blue, 0.8);
    gluDisk(quad, 0, objsize, 30, 1);
    glColor4d(0,0,0, 0.8);
    gluDisk(quad, 0, objsize * .9, 30, 1);
    glPopMatrix();
} 

void Circle::updateBBox()
{
    bbox.min_x = x - size;
    bbox.min_y = y - size;
    bbox.max_x = x + size;
    bbox.max_y = y + size;
}
