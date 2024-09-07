#include "Pellet.h"

Pellet::Pellet(float _posX, float _posY) : posX(_posX), posY(_posY) {
	pelletRect = { static_cast<int>(posX), static_cast<int>(posY), PELLET_SIZE, PELLET_SIZE };
}

float Pellet::getX() {
	return posX;
}

float Pellet::getY() {
	return posY;
}

SDL_Rect Pellet::getRect() {
	return pelletRect;
}

void Pellet::render(SDL_Renderer* rd) {
	drawPellet(rd, posX, posY, PELLET_SIZE);
}
