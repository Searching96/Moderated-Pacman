#pragma once
#include <SDL.h>
#include <vector>
#include "Const.h"
#include "Pellet.h"
#include "Draw.h"

using namespace std;

class Pacman {
private:
	SDL_Rect pacmanRect;
	float speed;
	int dirX, dirY;
	float posX, posY;
	int tempDirX, tempDirY;
	int mouthState;
	Uint32 lastMouthChange;
public:
	Pacman(int _x, int _y, int _w, int _h, float _speed);
	void handleEvent(SDL_Event& ev);
	float getPosX();
	float getPosY();
	void render(SDL_Renderer* rd);
	void move(const vector<vector<int>>& map, vector<Pellet>& pellets);
	bool isValidMove(float newX, float newY, const vector<vector<int>>& map);
	void wrapAround(float& x, float& y);
	void checkPelletCollision(vector<Pellet>& pellets);
	SDL_Rect getRect();
};