#include "Ghost.h"

Ghost::Ghost(int _x, int _y, int _w, int _h, float _speed, 
             float _chaseDistance, SDL_Renderer* rd, const string& imagePath)
    : speed(_speed), dirX(0), dirY(0), posX(static_cast<float>(_x)), 
      posY(static_cast<float>(_y)), tempDirX(0), tempDirY(0), chaseDistance(_chaseDistance){
    ghostRect = { _x, _y, _w, _h };
    init(rd, imagePath);
}

Ghost::~Ghost() {
    cleanup();
}

bool Ghost::isValidMove(float newX, float newY, const vector<vector<int>>& map) {
    if (newX < 0 || newX + ghostRect.w > MAP_WIDTH || newY < 0 || newY + ghostRect.h > MAP_HEIGHT)
        return false;
}

void Ghost::randomMove(const vector<vector<int>>& map) {
    vector<pair<int, int>> possibleMoves;

    if (isValidMove(posX, posY - speed, map)) {
        possibleMoves.push_back(make_pair(0, -1)); // up
    }
    if (isValidMove(posX, posY + speed, map)) {
        possibleMoves.push_back(make_pair(0, 1)); // down
    }
    if (isValidMove(posX - speed, posY, map)) {
        possibleMoves.push_back(make_pair(-1, 0)); // left
    }
    if (isValidMove(posX + speed, posY, map)) {
        possibleMoves.push_back(make_pair(1, 0)); // right
    }

    if (!possibleMoves.empty()) {
        srand(static_cast<unsigned int>(time(0)));
        int randomIndex = rand() % possibleMoves.size();
        dirX = possibleMoves[randomIndex].first;
        dirY = possibleMoves[randomIndex].second;
    }
}

void Ghost::chasePacman(Pacman* pacman, const vector<vector<int>>& map) {
    float distUp = calculateDistance(posX, posY - speed, pacman->getPosX(), pacman->getPosY());
    float distDown = calculateDistance(posX, posY + speed, pacman->getPosX(), pacman->getPosY());
    float distLeft = calculateDistance(posX - speed, posY, pacman->getPosX(), pacman->getPosY());
    float distRight = calculateDistance(posX + speed, posY, pacman->getPosX(), pacman->getPosY());

    // if there is a direction to chase
    if ((distUp < chaseDistance || distDown < chaseDistance || distLeft < chaseDistance || distRight < chaseDistance) 
        && (isValidMove(posX, posY - speed, map) || isValidMove(posX, posY + speed, map) 
            || isValidMove(posX - speed, posY, map) || isValidMove(posX + speed, posY, map))) {
        if (distUp <= distDown && distUp <= distLeft && distUp <= distRight && isValidMove(posX, posY - speed, map)) {
            dirX = 0;
            dirY = -1; // chase up
        }
        else if (distDown <= distUp && distDown <= distLeft && distDown <= distRight && isValidMove(posX, posY + speed, map)) {
            dirX = 0;
            dirY = 1; // chase down
        }
        else if (distLeft <= distUp && distLeft <= distDown && distLeft <= distRight && isValidMove(posX - speed, posY, map)) {
            dirX = -1;
            dirY = 0; // chase left
        }
        else {
            dirX = 1;
            dirY = 0; // chase right
        }
    }
    // no direction to chase
    else {
        randomMove(map);
    }

    // move ghost
    posX += dirX * speed;
    posY += dirY * speed;

    wrapAround();

    // update rect
    ghostRect.x = static_cast<int>(posX);
    ghostRect.y = static_cast<int>(posY);
}

void Ghost::wrapAround() {
    if (posX < 0) posX = MAP_WIDTH - 1;
    if (posX >= MAP_WIDTH) posX = 0;
    if (posY < 0) posY = MAP_HEIGHT - 1;
    if (posY >= MAP_HEIGHT) posY = 0;
}

float Ghost::calculateDistance(float x1, float y1, float x2, float y2) {
    return static_cast<float>(sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2)));
}

void Ghost::render(SDL_Renderer* rd) {
    // Set the rendering space and render to screen
    SDL_Rect renderQuad = { static_cast<int>(posX), static_cast<int>(posY), ghostRect.w, ghostRect.h };

    // Render to screen
    SDL_RenderCopy(rd, texture, NULL, &renderQuad);

    // Optional: Draw the chase distance circle
    drawChaseDistance(rd);
}

void Ghost::drawChaseDistance(SDL_Renderer* rd) {
    int centerX = static_cast<int>(posX) + ghostRect.w / 2;
    int centerY = static_cast<int>(posY) + ghostRect.h / 2;
    SDL_SetRenderDrawColor(rd, 255, 0, 0, 255); 
    drawCircle(rd, static_cast<float>(centerX), static_cast<float>(centerY), static_cast<int>(chaseDistance));
}

SDL_Rect Ghost::getRect() {
    return ghostRect;
}

bool Ghost::checkPacmanCollision(Pacman* pacman) {
    SDL_Rect pacmanRect = pacman->getRect();
    return (SDL_HasIntersection(&ghostRect, &pacmanRect));
}

bool Ghost::init(SDL_Renderer* rd, const string& imagePath) {
    SDL_Surface* surface = IMG_Load(imagePath.c_str());
    if (!surface) {
        SDL_Log("Unable to load image %s! SDL_image Error: %s\n", imagePath.c_str(), IMG_GetError());
        return false;
    }

    texture = SDL_CreateTextureFromSurface(rd, surface);
    SDL_FreeSurface(surface);

    if (!texture) {
        SDL_Log("Unable to create texture from %s! SDL Error: %s\n", imagePath.c_str(), SDL_GetError());
        return false;
    }

    return true;
}

void Ghost::cleanup() {
    if (texture) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
}