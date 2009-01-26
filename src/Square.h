#ifndef __SQUARE_H
#define __SQUARE_H

#include "Item.h"

class Square : public Item
{
    public:
        Square(int xpos = 250, int ypos = 250, int size = 15);
        ~Square();
        void draw();
        void drawBBox();
        void updateBBox();
};
#endif
