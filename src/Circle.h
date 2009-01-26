#ifndef __CIRCLE_H
#define __CIRCLE_H

#include "Item.h"

class Circle : public Item
{
    public:
        Circle(int xpos = 250, int ypos = 250, int size = 15);
        ~Circle();
        void draw();
        void drawBBox();
        void updateBBox();

    private:
        GLUquadric* quad;
};
#endif
