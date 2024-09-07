#pragma once
#include <cmath>
#include <SDL.h>
#include "Const.h"

using namespace std;

void drawPoint(SDL_Renderer* rd, int x, int y);
void drawCircle(SDL_Renderer* rd, float centerX, float centerY, int radius);
void drawFilledCircle(SDL_Renderer* rd, int centerX, int centerY, int radius, SDL_Color color);
void drawTransparentCircle(SDL_Renderer* rd, float x, float y, int radius, SDL_Color color);
void drawGhostBody(SDL_Renderer* rd, int x, int y, int width, int height);
void drawGhostEyes(SDL_Renderer* rd, int x, int y, int width, int height);
void drawPacman(SDL_Renderer* rd, int centerX, int centerY, int radius, int startAngle, int endAngle, SDL_Color bodyColor);
void drawPellet(SDL_Renderer* rd, int centerX, int centerY, int radius);
void drawTile(SDL_Renderer* rd, int x, int y, int type);