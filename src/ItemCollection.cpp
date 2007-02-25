#include "ItemCollection.h"
#include "Square.h"
#include <iostream>

ItemCollection::ItemCollection()
{
    head = new Item();
    selected = head;
    head->next = NULL;
    head->previous = NULL;
    tail = head;
}


ItemCollection::~ItemCollection()
{
}


void ItemCollection::push(Item* n)
{
    Item* cur = head;

    while (cur->next != NULL)
    {
        cur = cur->next;
    }
    cur->next = n;
    n->previous = cur;
    tail = n;
}

void ItemCollection::removeItem(Item* i)
{
    Item* cur = head;

    while (cur != NULL)
    {
        if (cur == i)
        {
            std::cout << "found an item matching i\n";
            if (cur->previous != NULL)
            {
                cur->previous->next = cur->next;
                if (cur->next != NULL)
                    cur->next->previous = cur->previous;

                if (cur == tail)
                    tail = cur->previous;

                return;
            }
            //else they're trying to delete head, which we won't allow.
        }
        cur = cur->next;
    }
}

void ItemCollection::pop()
{
    if (tail->previous != NULL)
    {
        tail = tail->previous;
        delete tail->next;
        tail->next = NULL;
    }
}

void ItemCollection::drawAll()
{
    Item* cur = head;
    while (cur != NULL)
    {
        cur->rotate();
        //cur->moveTo((cur->getx() + cur->getMomentumX()), (cur->gety() + cur->getMomentumY()));
        cur->draw();
        cur = cur->next;
    }
}

void ItemCollection::select(GLdouble x, GLdouble y)
{
    /* Translate x and y into world coordinates */
    GLdouble objz = 0;
    GLUquadric* quad = gluNewQuadric();
    GLdouble modelMatrix[16];
    GLdouble projMatrix[16];
    GLint viewport[4];
    glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
    glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);
    glGetIntegerv(GL_VIEWPORT, viewport);
    //(x and y are translated here.)
    gluUnProject(x, y, 0, modelMatrix, projMatrix, viewport, &x, &y, &objz);

    Item* cur = tail;
    bool done = false;
    while (cur != NULL && !done)
    {
        done = true;

        /* Is this what we tried to click on? */
        if (x <= (cur->getobjx() + cur->getSize()) &&
                x >= (cur->getobjx() - cur->getSize()) &&
                y <= (cur->getobjy() + cur->getSize()) &&
                y >= (cur->getobjy() - cur->getSize()))
        {
            selected->setColor(1,1,1);
            selected = cur;
        }
        else
        {
            /* Nope, try the next one. */
            done = false;
            cur = cur->previous;
        }
    }
    /* If we haven't found one yet... */
    if (cur == NULL)
    {
        /* Select the last item of the list. */
        selected->setColor(1,1,1);
        selected = tail;
        cur = tail;
    }

    /* Now we set the selected one as the topmost one. */
    if (cur->next != NULL)
    {
        Item *tmp = cur;
        if (tmp->previous != NULL)
            tmp->previous->next = tmp->next;
        tmp->next->previous = tmp->previous;
        tail->next = tmp;
        tmp->previous = tail;
        tmp->next = NULL;
        tail = tmp;
    }

    selected->setColor( .8, 0, 0);
}

Item* ItemCollection::getSelected()
{
    return selected;
}
