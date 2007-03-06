#include "Square.h"

Square::Square()
{
    x = 250;
    y = 250;
    spin = 0;
    size = 10;
    next = NULL;
    objx = 0;
    objy = 0;
}

Square::Square(int xpos, int ypos, int size)
{
    this->x = xpos;
    this->y = ypos;
    this->size = size;
    spin = 0;
    size = 10;
    next = NULL;
}

Square::~Square()
{
}

void Square::draw()
{
    drawBBox();
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
    glRotatef(spin, 0, 0, objz);
    glColor3f(red, green, blue);
    glRectf(size, -size, -size, size);
    glColor3f(0, 0, 0);
    glRectf(size*.9, -(size*.9), -(size*.9), size*.9);
    glPopMatrix();
    //glutPostRedisplay();
} 

void Square::drawBBox()
{
    GLdouble objz = 0;
    GLdouble modelMatrix[16];
    GLdouble projMatrix[16];
    GLint viewport[4];

    GLdouble csize = sqrt((size * size) + (size * size));
    GLdouble theta = ((((int)spin % 90) + 45) * (2.0 * PI)) / 360.0; //theta is spin in radians
    GLdouble w, h;
    w = csize * cos(theta);
    h = csize * sin(theta);

    w = h = max(w,h);

    glPushMatrix();
    glLoadIdentity();

    glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
    glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);
    glGetIntegerv(GL_VIEWPORT, viewport);

    gluUnProject(x, y, 0, modelMatrix, projMatrix, viewport, &objx, &objy, &objz);

    /* Move the square to where it belongs */
    glTranslatef(objx, -objy, objz);
    glColor3f(red * 0.5, green * 0.5, blue * 0.5);
    glRectf(w, h, -w, -h);
    //glRectf(10,-10, -10, 10);
    glPopMatrix();
} 
