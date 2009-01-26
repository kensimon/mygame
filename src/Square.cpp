#include "Game.h"
#include "Square.h"

Square::Square(int xpos, int ypos, int size)
{
    x = xpos;
    y = ypos;
    size = size;
    degrees = 0;
	floor_friction = 0.75;
	elasticity = 0.75;
}

Square::~Square()
{
}

void Square::draw()
{
    glPushMatrix();
    glLoadIdentity();

    glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
    glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);
    glGetIntegerv(GL_VIEWPORT, viewport);

    gluUnProject(x, y, 0, modelMatrix, projMatrix, viewport, &objx, &objy, &objz);

    /* Move the square to where it belongs */
    glTranslated(objx, -objy, objz);
    glRotated(degrees, 0, 0, objz);
    glColor4d(red, green, blue, 0.8);
    glRectd(size, -size, -size, size);
    glColor4d(0.0, 0.0, 0.0, 0.8);
    glRectd(size*.9, -(size*.9), -(size*.9), size*.9);
    glPopMatrix();
} 

void Square::updateBBox()
{
    GLdouble csize = sqrt((size * size) + (size * size));
    GLdouble theta = ((((int)degrees % 90) + 45) * (2.0 * PI)) / 360.0;
    GLdouble w, h;
    w = csize * cos(theta);
    h = csize * sin(theta);

    w = h = max(w,h);
    bbox.min_x = x - w;
    bbox.min_y = y - h;
    bbox.max_x = x + w;
    bbox.max_y = y + h;
}
