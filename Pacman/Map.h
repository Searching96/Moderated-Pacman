#pragma once
#include <SDL.h>
#include <vector>
#include "Const.h"
#include "Draw.h"

using namespace std;

class Map
{
private:
	vector<vector<int>> map;
public:
	Map();
	void loadMap(const vector<vector<int>>& _map);
	void render(SDL_Renderer* rd);
	vector<vector<int>> getMap();
};
