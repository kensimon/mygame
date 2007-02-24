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
    GLdouble objz = 0;
    GLdouble modelMatrix[16];
    GLdouble projMatrix[16];
    GLint viewport[4];

    glPushMatrix();

    /* OpenGL is fucking retarded.  Why does everything return void? 
     * I'd love to call GLdouble modelMatrix[16] = glGetDoublev(GL_MODELVIEW_MATRIX),
     * but it insists on making me type extra lines.
     */
    glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
    glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);
    glGetIntegerv(GL_VIEWPORT, viewport);

    /* All that for this line. */
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
