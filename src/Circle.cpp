#include "Circle.h"

Circle::Circle()
{
    x = 250;
    y = 250;
    spin = 0;
    size = 10;
    next = NULL;
    objx = 0;
    objy = 0;
}

Circle::Circle(int xpos, int ypos, int size)
{
    this->x = xpos;
    this->y = ypos;
    this->size = size;
    spin = 0;
    size = 10;
    next = NULL;
}

Circle::~Circle()
{
}

void Circle::draw()
{
    GLdouble objz = 0;
    GLUquadric* quad = gluNewQuadric();
    GLdouble modelMatrix[16];
    GLdouble projMatrix[16];
    GLint viewport[4];
    glPushMatrix();

    glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
    glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);
    glGetIntegerv(GL_VIEWPORT, viewport);
    gluUnProject(x, y, 0, modelMatrix, projMatrix, viewport, &objx, &objy, &objz);

    glTranslatef(objx, -objy, objz);
    glRotatef(spin, 0, 0, objz);
    glColor3f(red, green, blue);
    gluDisk(quad, 0, size, 30, 1);
    glColor3f(0,0,0);
    gluDisk(quad, 0, size*.9, 30, 1);
    glPopMatrix();
} 
