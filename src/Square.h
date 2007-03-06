#ifndef __SQUARE_H
#define __SQUARE_H

#include "Item.h"

class Square : public Item
{
    public:
        Square();
        Square(int xpos, int ypos, int size);
        ~Square();
        void draw();
        void drawBBox();
};
#endif
