#include "ItemCollection.h"
#include "Square.h"
#include "Game.h"
#include <iostream>

/*
 * ItemCollection is a double-linked list of Item objects.
 */
ItemCollection::ItemCollection()
{
    head = new Item();
    selected = head;
    head->next = NULL;
    head->previous = NULL;
    tail = head;
    _length = 0;
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
    _length++;
}

Item* ItemCollection::get(int num)
{
    Item* cur = head->next;
    int i = 0;

    while (cur != NULL)
    {
        if (i == num)
            return cur;
        i++;
        cur = cur->next;
    }

    return NULL;
}

void ItemCollection::removeItem(Item* i)
{
    Item* cur = head;

    while (cur != NULL)
    {
        if (cur == i)
        {
            if (cur->previous != NULL)
            {
                cur->previous->next = cur->next;
                if (cur->next != NULL)
                    cur->next->previous = cur->previous;

                if (cur == tail)
                    tail = cur->previous;

                delete cur;
                _length--;
				/* If we just deleted selected, make sure to reset what selected is (head) */
				if (cur == selected)
					selected = head;
                return;
            }
            //else they're trying to delete head, which we won't allow.
        }
        cur = cur->next;
    }
}

void ItemCollection::removeItem(int num)
{
    Item* cur = head->next;
    int i = 0;

    while (cur != NULL)
    {
        if (i == num)
        {
            if (cur->previous != NULL)
            {
                cur->previous->next = cur->next;
                if (cur->next != NULL)
                    cur->next->previous = cur->previous;

                if (cur == tail)
                    tail = cur->previous;

				if (cur == selected)
					selected = head;
                delete cur;
                _length--;
                return;
            }
            //else they're trying to delete head, which we won't allow.
        }
        cur = cur->next;
        i++;
    }
}

void ItemCollection::pop()
{
    if (tail->previous != NULL)
    {
		if (tail == selected)
			selected = head;
        tail = tail->previous;
        delete tail->next;
        tail->next = NULL;
        _length++;
    }
}

void ItemCollection::drawAll()
{
    bool dbb = Game::getInstance()->drawBBoxes;
    Item* cur = head;
    while (cur != NULL)
    {
        if (dbb) {
            cur->drawBBox();
        }
        cur->draw();
        cur = cur->next;
    }
}

void ItemCollection::select(GLdouble x, GLdouble y)
{
    /* Translate x and y into world coordinates */
    GLdouble objz = 0;
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
        /* Select nothing. */
		if (selected != head)
			selected->setColor(1,1,1);
        selected = head;
        cur = head;
    }

    /* 
     * Now we set the selected one as the topmost one,
     * so long as it's not head.
     */
    if (cur->next != NULL && cur != head)
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

    if (selected != head)
        selected->setColor( .8, 0, 0);
}

Item* ItemCollection::getSelected()
{
    return selected;
}

int ItemCollection::length()
{
    return _length;
}
