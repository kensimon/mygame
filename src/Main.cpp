#include "Game.h"

int main(int argc, char **argv)
{
    int retval;
    retval = Game::getInstance()->init(argc, argv);
    return retval;
}
