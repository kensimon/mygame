#include "Game.h"
#include "Square.h"
#include "Circle.h"
#include "ItemCollection.h"
#include "Config.h"
#include <iostream>

Game* Game::instance = NULL;

Game::Game()
{
    ic = new ItemCollection();
    phys = new Physics(ic);
    drawBBoxes = false;
    framewait = 16;
    curbutton = 0;
    width = WINSIZE_X;
    height = WINSIZE_Y;
};

Game::~Game()
{
};

Game* Game::getInstance()
{
    if (instance == NULL)
        instance = new Game();
    return instance;
}

int Game::init(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(width, height);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("mygame");
    glClearColor(0.0, 0.0, 0.8, 0.0);
    glShadeModel(GL_FLAT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glutDisplayFunc(Game::display);
    glutReshapeFunc(Game::reshape);
    glutMouseFunc(Game::mouse);
    glutKeyboardFunc(Game::keyboardFunc);
    glutSpecialFunc(Game::specialFunc);
    glutTimerFunc(Game::framewait, Game::timerFunc, 0);
    glutMotionFunc(Game::dragMouse);
    glutMainLoop();
    return 0;
}

void Game::timerFunc(int)
{
    instance->phys->tick();
    Game::display();
    GLenum error = glGetError();
    if (error != GL_NO_ERROR)
        printf ("%s\n", error);
    glutTimerFunc(instance->framewait, Game::timerFunc, 0);
}

void Game::display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    instance->ic->drawAll();
    glutSwapBuffers();
}

void Game::reshape(int w, int h)
{
    instance->width = w;
    instance->height = h;
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w <= h)
        gluOrtho2D (-1.0, 1.0,
                -1.0*(GLfloat)h/(GLfloat)w, 1.0*(GLfloat)h/(GLfloat)w);
    else
        gluOrtho2D (-1.0*(GLfloat)w/(GLfloat)h,
                1.0*(GLfloat)w/(GLfloat)h, -1.0, 1.0);

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
            printf("x: %f, y: %f\n", instance->ic->getSelected()->getx(), instance->ic->getSelected()->gety());
            printf("spin: %f\n", instance->ic->getSelected()->getSpin());
            printf("momentumx: %f, momentumy: %f\n\n", instance->ic->getSelected()->momentumX,
                    instance->ic->getSelected()->momentumY);
            break;
        case 'd':
            instance->ic->removeItem(instance->ic->getSelected());
            break;
        case 'g':
            instance->phys->gravityOn = !instance->phys->gravityOn;
            break;
        case 'p':
            instance->ic->pop();
            break;
        case 'b':
            instance->drawBBoxes = !(instance->drawBBoxes);
            break;
        case 'l':
            instance->ic->removeItem(instance->ic->length() - 1);
            break;
        case '0':
            instance->ic->removeItem(0);
            break;
        case '1':
            instance->ic->removeItem(1);
            break;
        case '2':
            instance->ic->removeItem(2);
            break;
        case '3':
            instance->ic->removeItem(3);
            break;
        case '4':
            instance->ic->removeItem(4);
            break;
        case '5':
            instance->ic->removeItem(5);
            break;
        case '6':
            instance->ic->removeItem(6);
            break;
        case '7':
            instance->ic->removeItem(7);
            break;
        case '8':
            instance->ic->removeItem(8);
            break;
        case '9':
            instance->ic->removeItem(9);
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
            instance->ic->getSelected()->resize(1);
            break;
        case GLUT_KEY_DOWN:
            instance->ic->getSelected()->resize(-1);
            break;
        case GLUT_KEY_LEFT:
            instance->ic->getSelected()->spinMomentum += 1;
            break;
        case GLUT_KEY_RIGHT:
            instance->ic->getSelected()->spinMomentum -= 1;
            break;
        default:
            break;
    }
}

void Game::mouse(int button, int state, int x, int y)
{
    instance->curbutton = button;
    switch (instance->curbutton) {
        case GLUT_LEFT_BUTTON:
            if (state == GLUT_DOWN)
            {
                instance->ic->select((GLdouble)x, (GLdouble)y);
                instance->ic->getSelected()->setClickPos((GLdouble)x, (GLdouble)y);
                instance->ic->getSelected()->grabbed = true;
            }
            if (state == GLUT_UP)
            {
                instance->ic->getSelected()->grabbed = false;
            }
            break;
        case GLUT_RIGHT_BUTTON:
            if (state == GLUT_DOWN)
            {
                //instance->ic->getSelected()->moveTo(x, y);
                instance->ic->push(new Circle(x, y));
            }
            break;
        case GLUT_MIDDLE_BUTTON:
            if (state == GLUT_DOWN)
            {
                instance->ic->push(new Square(x, y));
            }
        default:
            break;
    }
}

void Game::dragMouse(int x, int y)
{
        if (instance->curbutton == GLUT_LEFT_BUTTON)
        {
            instance->ic->getSelected()->dragTo(x, y);
        }
}

ItemCollection* Game::getItemCollection()
{
    return this->ic;
}

Physics* Game::getPhysicsEngine()
{
    return this->phys;
}

int Game::getWidth()
{
    return width;
}

int Game::getHeight()
{
    return height;
}
