#include "MainGame.h"

int main(int argc, char** argv) {
	MainGame mainGame("Zombie Game", 800, 700); //1024, 768
    mainGame.run();

    return 0;
}