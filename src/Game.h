#ifndef __GAME_H
#define __GAME_H
#include <GL/glut.h>
#include <iostream>

class Game
{
    public:
        Game();
        ~Game();
        static Game *getInstance();
        int init(int, char**);

    private:
        static void display();
        static void reshape(int, int);
        static void mouse(int, int, int, int);
        static void keyboardFunc(unsigned char, int, int);
        static void specialFunc(int, int, int);
        static void timerFunc(int);
        static void dragMouse(int, int);
        static Game *instance;
};
#endif
