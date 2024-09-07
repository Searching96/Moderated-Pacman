#pragma once
#include <SDL.h>
#include "Const.h"
#include "Draw.h"

using namespace std;

class Pellet {
private:
	float posX, posY;
	SDL_Rect pelletRect;
public:
	Pellet(float _posX, float _posY);
	float getX();
	float getY();
	SDL_Rect getRect();
	void render(SDL_Renderer* rd);
};

