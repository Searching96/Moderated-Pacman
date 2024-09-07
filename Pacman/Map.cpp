#include "Map.h"

Map::Map() {

}

void Map::loadMap(const vector<vector<int>>& _map) {
	map = _map;
}

void Map::render(SDL_Renderer* rd) {
	for (int row = 0; row < map.size(); row++)
		for (int col = 0; col < map[row].size(); col++)
			drawTile(rd, col * TILE_SIZE, row * TILE_SIZE, map[row][col]);
}

vector<vector<int>> Map::getMap() {
	return map;
}