#ifndef __GAME_H
#define __GAME_H
#include <GL/glut.h>
#include <iostream>
#include "ItemCollection.h"
#include <boost/thread.hpp>

using boost::mutex;

class Game
{
    public:
        static Game* getInstance();
		bool isRendering;
		bool isCalculating;
        int init(int, char**);
        bool drawBBoxes;
        ItemCollection* getItemCollection();
        int getHeight();
        int getWidth();
		bool getGravityOn();
		mutex stdout_mutex;

    private:
        static void display();
        static void reshape(int, int);
        static void mouse(int, int, int, int);
        static void keyboardFunc(unsigned char, int, int);
        static void specialFunc(int, int, int);
        static void drawTimerCallback(int);
		static void physTimerCallback(int);
        static void dragMouse(int, int);
        static Game* instance;
        int curbutton;
        int framewait;
        ItemCollection* ic;
        int width;
        int height;
		mutex read_mutex;
		bool gravityOn;


    protected:
        Game();
        virtual ~Game();
};
#endif
