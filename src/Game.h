#ifndef __GAME_H
#define __GAME_H
#include <GL/glut.h>
#include <iostream>
#include "ItemCollection.h"
#include "Physics.h"

class Game
{
    public:
        static Game* getInstance();
        int init(int, char**);
        bool drawBBoxes;

    private:
        static void display();
        static void reshape(int, int);
        static void mouse(int, int, int, int);
        static void keyboardFunc(unsigned char, int, int);
        static void specialFunc(int, int, int);
        static void timerFunc(int);
        static void dragMouse(int, int);
        static Game* instance;
        int curbutton;
        int framewait;
        ItemCollection* ic;
        Physics* phys;

    protected:
        Game();
        virtual ~Game();
};
#endif
