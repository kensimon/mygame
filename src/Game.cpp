#include "Game.h"
#include "Square.h"
#include "Circle.h"
#include "EntityList.h"
#include "Config.h"
#include <iostream>
#include <cstdlib>
#include <list>
#include <cstdio>

Game* Game::instance = NULL;

Game::Game()
{
    drawBBoxes = false;
    el = new EntityList(16666);
    curbutton = 0;
    width = WINSIZE_X;
    height = WINSIZE_Y;
	isRendering = false;
	isCalculating = false;
	gravityOn = true;
};

Game::~Game()
{
	el->stopCalculating();
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
    glClearColor((GLclampf)0.0, (GLclampf)0.0, (GLclampf)0.8, (GLclampf)0.0);
    glShadeModel(GL_FLAT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glutDisplayFunc(Game::display);
    glutReshapeFunc(Game::reshape);
    glutMouseFunc(Game::mouse);
    glutKeyboardFunc(Game::keyboardFunc);
    glutSpecialFunc(Game::specialFunc);
    glutTimerFunc(0, Game::drawTimerCallback, 0);
    glutMotionFunc(Game::dragMouse);
	el->startCalculating();
    glutMainLoop();
    return 0;
}

void Game::drawTimerCallback(int)
{
	if (instance->isRendering)
		return;
	instance->isRendering = true;
	glutTimerFunc(16, Game::drawTimerCallback, 0);
	/* Start timer functions here */
    Game::display();
    GLenum error = glGetError();
    if (error != GL_NO_ERROR)
        printf ("%s\n", (char*)error);

	/* End timer functions here */
	instance->isRendering = false;
}

void Game::display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    instance->el->drawAll();
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
			{
				if (instance->el->getSelected() == NULL)
				{
					break;
				}
				printf("x: %f, y: %f\n", instance->el->getSelected()->getx(), instance->el->getSelected()->gety());
				printf("rotation: %f\n", instance->el->getSelected()->getRotation());
				printf("momentumx: %f, momentumy: %f\n\n", instance->el->getSelected()->momentumX,
						instance->el->getSelected()->momentumY);
				break;
			}
        case 'd':
            instance->el->removeEntity(instance->el->getSelected());
            break;
        case 'g':
            instance->gravityOn = !instance->gravityOn;
            break;
        case 'p':
            instance->el->pop_back();
            break;
        case 'b':
            instance->drawBBoxes = !(instance->drawBBoxes);
            break;
        case 'l':
            instance->el->removeEntity(instance->el->length() - 1);
            break;
		case 's':
			{
				if (instance->el->isCalculationStopped())
					instance->el->startCalculating();
				else
					instance->el->stopCalculating();
				break;
			}
        case '0':
            instance->el->removeEntity(0);
            break;
        case '1':
            instance->el->removeEntity(1);
            break;
        case '2':
            instance->el->removeEntity(2);
            break;
        case '3':
            instance->el->removeEntity(3);
            break;
        case '4':
            instance->el->removeEntity(4);
            break;
        case '5':
            instance->el->removeEntity(5);
            break;
        case '6':
            instance->el->removeEntity(6);
            break;
        case '7':
            instance->el->removeEntity(7);
            break;
        case '8':
            instance->el->removeEntity(8);
            break;
        case '9':
            instance->el->removeEntity(9);
            break;
        default:
            break;
    }
}

void Game::specialFunc(int key, int x, int y)
{
	if (instance->el->getSelected() == NULL)
	{
		return;
	}
    switch(key)
    {
        case GLUT_KEY_UP:
            instance->el->getSelected()->resize(1);
            break;
        case GLUT_KEY_DOWN:
            instance->el->getSelected()->resize(-1);
            break;
        case GLUT_KEY_LEFT:
            instance->el->getSelected()->spinMomentum -= 1;
            break;
        case GLUT_KEY_RIGHT:
            instance->el->getSelected()->spinMomentum += 1;
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
                instance->el->select((GLdouble)x, (GLdouble)y);
				Entity* selected = instance->el->getSelected();
				if (selected != NULL)
				{
					instance->el->getSelected()->setClickPos((GLdouble)x, (GLdouble)y);
					instance->el->getSelected()->grabbed = true;
				}
            }
            if (state == GLUT_UP)
            {
				if (instance->el->getSelected() != NULL)
				{
					instance->el->getSelected()->grabbed = false;
				}
            }
            break;
        case GLUT_RIGHT_BUTTON:
            if (state == GLUT_DOWN)
            {
                instance->el->push_back(new Circle(instance->el->getNextEntityId(), x, y));
            }
            break;
        case GLUT_MIDDLE_BUTTON:
            if (state == GLUT_DOWN)
            {
                instance->el->push_back(new Square(instance->el->getNextEntityId(), x, y));
            }
        default:
            break;
    }
}

void Game::dragMouse(int x, int y)
{
	if (instance->curbutton == GLUT_LEFT_BUTTON && instance->el->getSelected() != NULL)
	{
		instance->el->getSelected()->dragTo(x, y);
	}
}

EntityList* Game::getEntityList()
{
    return this->el;
}

int Game::getWidth()
{
	mutex::scoped_lock lock(read_mutex);
    return width;
}

int Game::getHeight()
{
	mutex::scoped_lock lock(read_mutex);
    return height;
}

bool Game::getGravityOn()
{
	mutex::scoped_lock lock(read_mutex);
	return gravityOn;
}
