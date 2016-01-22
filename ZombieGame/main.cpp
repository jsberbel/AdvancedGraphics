#include "MainGame.h"
#include <iostream>

int main(int argc, char** argv) {
	try {
		MainGame mainGame("Zombie Game", 800, 700); //1024, 768
		mainGame.run();
	}
	catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
		system("pause");
	}
	

    return 0;
}
