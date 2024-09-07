#pragma once
#include <SDL.h>
#include <string>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include "Pacman.h"
#include "Ghost.h"
#include "Map.h"
#include "Const.h"
#include "Pellet.h"

class Game {
private:
	SDL_Window* wd;
	SDL_Renderer* rd;
	bool quit;
	Pacman* pm;
	vector<Ghost*> ghosts;
	vector<Pellet> pellets;
	Map* map;
	int lives;
	unsigned int protectionTime;
	bool gameOver;

	void handleEvents();
	void update();
	void render();
public:
	Game();
	bool init();
	void run();
	void close();
	bool checkWin();
	void renderText(const string& message, int x, int y, int size, SDL_Color color);
	void checkGhostCollision();
	void DrawProtectionSphere();
};
