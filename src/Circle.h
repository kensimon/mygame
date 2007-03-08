#ifndef __CIRCLE_H
#define __CIRCLE_H

#include "Item.h"

class Circle : public Item
{
    public:
        Circle(int xpos, int ypos);
        Circle();
        ~Circle();
        void draw();
        void drawBBox();
        void updateBBox();
};
#endif
