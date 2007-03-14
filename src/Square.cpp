#include "Game.h"
#include "Square.h"

Square::Square()
{
    *this = Square(250, 250);
}

Square::Square(int xpos, int ypos)
{
    *this = Square(xpos, ypos, 3);
}

Square::Square(int xpos, int ypos, int size)
{
    this->x = xpos;
    this->y = ypos;
    this->size = size;
    spin = 0;
    next = NULL;
}

Square::~Square()
{
}

void Square::draw()
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
    glRotatef(spin, 0, 0, objz);
    glColor4f(red, green, blue, 0.8);
    glRectf(size, -size, -size, size);
    glColor4f(0.0, 0.0, 0.0, 0.8);
    glRectf(size*.9, -(size*.9), -(size*.9), size*.9);
    glPopMatrix();
    //glutPostRedisplay();
} 

void Square::updateBBox()
{
    GLdouble csize = sqrt((size * size) + (size * size));
    GLdouble theta = ((((int)spin % 90) + 45) * (2.0 * PI)) / 360.0;
    GLdouble w, h;
    w = csize * cos(theta);
    h = csize * sin(theta);

    w = h = max(w,h);
    bbox->min[0] = x - w;
    bbox->min[1] = y - h;
    bbox->max[0] = x + w;
    bbox->max[1] = y + h;
}
