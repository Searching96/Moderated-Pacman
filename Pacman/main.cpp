#include <SDL.h>
#include <iostream>
#include "Game.h"

using namespace std;

int main(int argc, char* argv[]) {
	Game game;
	if (!game.init()) {
		cerr << "Failed to initialize!" << endl;
		return -1;
	}

	game.run();
	game.close();
	return 0;
}