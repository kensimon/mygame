#include "Game.h"
#include "Square.h"
#include "Circle.h"
#include "ItemCollection.h"
#include <iostream>

static int framewait = 16;
ItemCollection *ic = new ItemCollection();
Game* Game::instance = 0;
int curbutton;

Game::Game()
{
};

Game::~Game()
{
};

Game* Game::getInstance()
{
    if (instance == 0)
        instance = new Game();
    return instance;
}

int Game::init(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("mygame");
    glClearColor(0.0, 0.0, 0.8, 0.0);
    glShadeModel(GL_FLAT);
    glutDisplayFunc(Game::display);
    glutReshapeFunc(Game::reshape);
    glutMouseFunc(Game::mouse);
    glutKeyboardFunc(Game::keyboardFunc);
    glutSpecialFunc(Game::specialFunc);
    glutTimerFunc(framewait, Game::timerFunc, 0);
    glutMotionFunc(Game::dragMouse);
    glutMainLoop();
    return 0;
}

void Game::timerFunc(int)
{
    Game::display();
    glutTimerFunc(framewait, Game::timerFunc, 0);
}

void Game::display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    ic->drawAll();
    glutSwapBuffers();
}

void Game::reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-50.0, 50.0, -50.0, 50.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void Game::keyboardFunc(unsigned char key, int, int)
{
    switch(key)
    {
        case 'q':
            exit(0);
            break;
        case 'c':
            printf("x: %f, y: %f\n", ic->getSelected()->getx(), ic->getSelected()->gety());
            printf("spin: %f\n\n", ic->getSelected()->getSpin());
            break;
        case 'd':
            ic->removeItem(ic->getSelected());
            break;
        case 'p':
            ic->pop();
            break;
        default:
            break;
    }
}

void Game::specialFunc(int key, int x, int y)
{
    switch(key)
    {
        case GLUT_KEY_UP:
            ic->getSelected()->resize(1);
            break;
        case GLUT_KEY_DOWN:
            ic->getSelected()->resize(-1);
            break;
        case GLUT_KEY_LEFT:
            ic->getSelected()->spinMomentum += 1;
            break;
        case GLUT_KEY_RIGHT:
            ic->getSelected()->spinMomentum -= 1;
            break;
        default:
            break;
    }
}

void Game::mouse(int button, int state, int x, int y)
{
    curbutton = button;
    switch (curbutton) {
        case GLUT_LEFT_BUTTON:
            if (state == GLUT_DOWN)
            {
                ic->select((GLdouble)x, (GLdouble)y);
            }
            break;
        case GLUT_RIGHT_BUTTON:
            if (state == GLUT_DOWN)
            {
                //ic->getSelected()->moveTo(x, y);
                ic->push(new Circle(x, y, 10));
            }
            break;
        case GLUT_MIDDLE_BUTTON:
            if (state == GLUT_DOWN)
            {
                ic->push(new Square(x, y, 10));
            }
        default:
            break;
    }
}

void Game::dragMouse(int x, int y)
{
    if (curbutton == GLUT_LEFT_BUTTON)
        ic->getSelected()->moveTo(x, y);

}
