#ifndef __SQUARE_H
#define __SQUARE_H

#include "Entity.h"

class Square : public Entity
{
    public:
        Square(int id, int xpos = 250, int ypos = 250, int size = 15);
        ~Square();
        void draw();
        void drawBBox();
        void updateBBox();
};
#endif
