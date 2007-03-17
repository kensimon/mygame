#include "Game.h"
//#include <mcheck.h>

int main(int argc, char **argv)
{
    //mtrace();
    int retval;
    retval = Game::getInstance()->init(argc, argv);
    //muntrace();
    return retval;
}
