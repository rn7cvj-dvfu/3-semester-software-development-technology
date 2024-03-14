#include <cstdlib>
#include <ctime>
#include <signal.h>
#include <unistd.h>

#include "game.hpp"
#include "character.hpp"
#include "gui.hpp"

Game *g = NULL;

static void
interrupt_handler(int sign) {
    erase();
    endwin();
    g->running = false;
    GUI::Alert("You have closed the game, terminal will be cleaned soon.");
    exit(0);
}

int main(int argc, char **argv) {
    bool playagain;
    Character *c;

    srand(time(NULL));
    GUI::NUM_COLOURS = 8;
    GUI::Init();

    signal(SIGINT, interrupt_handler);

    do {
        erase();
        GUI::StartScreen();
        c = GUI::CharacterCreation();
        g = new Game(c);
        playagain = g->Run();
    } while (playagain);

    GUI::End();
    delete g;
    delete c;

    return 0;
}
