#ifndef __CIRCLE_H
#define __CIRCLE_H

#include "Entity.h"

class Circle : public Entity
{
    public:
        Circle(int id, int xpos = 250, int ypos = 250, int size = 15);
        ~Circle();
        void draw();
        void drawBBox();
        void updateBBox();

    private:
        GLUquadric* quad;
};
#endif
