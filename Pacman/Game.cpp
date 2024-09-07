#include "Game.h"
#include "Const.h"
#include <iostream>
using namespace std;

Game::Game() : wd(NULL), rd(NULL), quit(false), pm(NULL), map(NULL), lives(MAX_LIVES), protectionTime(0), gameOver(false) {}

bool Game::init() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << endl;
		return false;
	}

	if (TTF_Init() == -1) {
		cerr << "SDL_ttf could not initialize! TTF_Error: " << TTF_GetError() << endl;
		return false;
	}

	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags)) {
		cerr << "SDL_image could not initialize! IMG_Error: " << IMG_GetError() << endl;
		return false;
	}

	wd = SDL_CreateWindow("Pacman", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, MAP_WIDTH, MAP_HEIGHT, SDL_WINDOW_SHOWN);
	if (wd == NULL) {
		cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << endl;
		return false;
	}

	rd = SDL_CreateRenderer(wd, -1, SDL_RENDERER_ACCELERATED);
	if (rd == NULL) {
		cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << endl;
		return false;
	}

	map = new Map();
	map->loadMap(MAP);

	for (int y = 0; y < MAP.size(); y++)
		for (int x = 0; x < MAP[y].size(); x++)
			if (MAP[y][x] == 0)
				pellets.emplace_back(x * TILE_SIZE + TILE_SIZE / 2, y * TILE_SIZE + TILE_SIZE / 2);

	const float SPEED = 0.5f;

	pm = new Pacman(200, 200, 20, 20, SPEED);

	ghosts.push_back(new Ghost(20, 420, 20, 20, 0.350f * SPEED, 180, rd, "redghost.png"));
	ghosts.push_back(new Ghost(20, 20, 20, 20, 0.325f * SPEED, 200, rd, "pinkghost.png"));
	ghosts.push_back(new Ghost(380, 420, 20, 20, 0.375f * SPEED, 220, rd, "cyanghost.png"));
	ghosts.push_back(new Ghost(380, 20, 20, 20, 0.400f * SPEED, 240, rd, "orangeghost.png"));
	
	return true;
}

void Game::run() {
	while (!quit) {
		handleEvents();
		update();
		render();
	}
}

void Game::close() {
	delete pm;
	delete map;
	for (auto ghost : ghosts)
		delete ghost;
	SDL_DestroyRenderer(rd);
	SDL_DestroyWindow(wd);
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

// handle keyboard event
void Game::handleEvents() {
	SDL_Event ev;
	while (SDL_PollEvent(&ev) != 0) {
		if (ev.type == SDL_QUIT) {
			quit = true;
		}
		pm->handleEvent(ev);
	}
}

void Game::update() {
	unsigned int currentTime = SDL_GetTicks();
	pm->move(map->getMap(), pellets);
	if (currentTime > protectionTime)
		checkGhostCollision();
	for (auto ghost : ghosts)
		ghost->chasePacman(pm, map->getMap());
}

void Game::render() {
	SDL_SetRenderDrawColor(rd, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(rd);
	map->render(rd);
	for (auto pellet : pellets)
		pellet.render(rd);
	pm->render(rd);
	if (lives > 0)
		DrawProtectionSphere();
	for (auto ghost : ghosts)
		ghost->render(rd);
	SDL_Color color = { 100, 255, 100, 255 };
	Uint32 waitTime = 5000;
	renderText("Lives:" + to_string(lives), 10, 10, 16, color);
	if (checkWin()) {
		renderText("You Win!", 100, 100, 24, color);
		SDL_RenderPresent(rd);
		SDL_Delay(waitTime); 
		quit = true;
	}
	if (gameOver) {
		renderText("You Lose!", 100, 100, 24, color);
		SDL_RenderPresent(rd);
		SDL_Delay(waitTime);
		quit = true;
	}
	SDL_RenderPresent(rd);
}

bool Game::checkWin() {
	return pellets.empty();
}

void Game::renderText(const string& message, int x, int y, int size, SDL_Color color) {
	TTF_Font* font = TTF_OpenFont("Emulogic-zrEw.ttf", size);
	if (font == nullptr) {
		std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
		return;
	}

	SDL_Surface* surfaceMessage = TTF_RenderText_Solid(font, message.c_str(), color);
	SDL_Texture* messageTexture = SDL_CreateTextureFromSurface(rd, surfaceMessage);

	SDL_Rect messageRect;
	messageRect.x = x;
	messageRect.y = y;
	messageRect.w = surfaceMessage->w;
	messageRect.h = surfaceMessage->h;

	SDL_RenderCopy(rd, messageTexture, NULL, &messageRect);

	SDL_FreeSurface(surfaceMessage);
	SDL_DestroyTexture(messageTexture);
	TTF_CloseFont(font);
}

void Game::checkGhostCollision() {
	for (auto ghost : ghosts) {
		if (ghost->checkPacmanCollision(pm)) {
			lives--;
			protectionTime = SDL_GetTicks() + 3000;
			if (lives <= 0) {
				gameOver = true;
				break;
			}
		}
	}
}

void Game::DrawProtectionSphere() {
	Uint32 currentTime = SDL_GetTicks();
	if (currentTime < protectionTime) {
		SDL_Color green = { 0, 255, 0, 128 };
		drawTransparentCircle(rd, static_cast<int>(pm->getPosX()) + TILE_SIZE / 2,
						      static_cast<int>(pm->getPosY()) + TILE_SIZE / 2, 50, green);
	}
}