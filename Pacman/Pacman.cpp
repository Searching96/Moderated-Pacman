#include "Pacman.h"

Pacman::Pacman(int _x, int _y, int _w, int _h, float _speed)
    : speed(_speed), dirX(0), dirY(0),
      posX(static_cast<float>(_x)), posY(static_cast<float>(_y)),
      tempDirX(0), tempDirY(0), mouthState(0), lastMouthChange(0) {
    pacmanRect = { _x, _y, _w, _h };
}

void Pacman::handleEvent(SDL_Event& ev) {
    if (ev.type == SDL_KEYDOWN) {
        switch (ev.key.keysym.sym) {
        case SDLK_UP:
            tempDirX = 0;
            tempDirY = -1;
            break;
        case SDLK_DOWN:
            tempDirX = 0;
            tempDirY = 1;
            break;
        case SDLK_LEFT:
            tempDirX = -1;
            tempDirY = 0;
            break;
        case SDLK_RIGHT:
            tempDirX = 1;
            tempDirY = 0;
            break;
        default:
            break;
        }
    }
}

void Pacman::move(const vector<vector<int>>& map, vector<Pellet>& pellets) {
    // Real pos with temporary direction
    float tempNewX = posX + tempDirX * speed;
    float tempNewY = posY + tempDirY * speed;

    wrapAround(tempNewX, tempNewY);

    // Check if temporary direction is valid
    if (isValidMove(tempNewX, tempNewY, map)) {
        dirX = tempDirX;
        dirY = tempDirY;
    }

    // Real pos with current direction
    float newX = posX + dirX * speed;
    float newY = posY + dirY * speed;

    wrapAround(newX, newY);

    // If move is valid, update position
    if (isValidMove(newX, newY, map)) {
        posX = newX;
        posY = newY;
    }
    else {
        wrapAround(posX, posY);
    }

    // Update rect position
    pacmanRect.x = static_cast<int>(posX);
    pacmanRect.y = static_cast<int>(posY);

    checkPelletCollision(pellets);
}

void Pacman::wrapAround(float& x, float& y) {
    if (x < 0) x = MAP_WIDTH - 1;
    if (x >= MAP_WIDTH) x = 0;
    if (y < 0) y = MAP_HEIGHT - 1;
    if (y >= MAP_HEIGHT) y = 0;
}

bool Pacman::isValidMove(float newX, float newY, const vector<vector<int>>& map) {
    // compute tiles index
    int leftX = static_cast<int>(newX) / TILE_SIZE;
    int rightX = static_cast<int>(newX + pacmanRect.w - 1) / TILE_SIZE;
    int topY = static_cast<int>(newY) / TILE_SIZE;
    int bottomY = static_cast<int>(newY + pacmanRect.h - 1) / TILE_SIZE;

    // ensure valid index
    leftX = static_cast<int>(leftX + map[0].size()) % map[0].size();
    rightX = static_cast<int>(rightX + map[0].size()) % map[0].size();
    topY = static_cast<int>(topY + map.size()) % map.size();
    bottomY = static_cast<int>(bottomY + map.size()) % map.size();

    // check wall collision
    return !(map[topY][leftX] == 1 || map[topY][rightX] == 1 || map[bottomY][leftX] == 1 || map[bottomY][rightX] == 1);
}

void Pacman::render(SDL_Renderer* rd) {
    SDL_Color pacmanColor = { 255, 255, 0, 255 }; 

    int centerX = static_cast<int>(posX) + pacmanRect.w / 2;
    int centerY = static_cast<int>(posY) + pacmanRect.h / 2;
    int radius = pacmanRect.w / 2;

    float angle = atan2(-dirY, dirX) * (180.0f / M_PI); // rad to degree
    if (angle < 0) angle += 360; // ensure angle in range (0, 360)

    // change mouth size over time
    Uint32 currentTime = SDL_GetTicks();
    if (currentTime - lastMouthChange > 400) { // change every 400ms
        mouthState = (mouthState + 1) % 3; // change between 0, 1, 2
        lastMouthChange = currentTime;
    }

    int mouthWidth = 60; // max mouth size
    mouthWidth = mouthWidth * (2 - mouthState) / 2; // adjust mouth size

    int startAngle = static_cast<int>(angle - mouthWidth / 2);
    int endAngle = static_cast<int>(angle + mouthWidth / 2);

    // handle mouth size >= 360
    if (endAngle >= 360) {
        drawPacman(rd, centerX, centerY, radius, startAngle, 360, pacmanColor);
        drawPacman(rd, centerX, centerY, radius, 0, endAngle - 360, pacmanColor);
    }
    else {
        drawPacman(rd, centerX, centerY, radius, startAngle, endAngle, pacmanColor);
    }
}

float Pacman::getPosX() {
    return posX;
}

float Pacman::getPosY() {
    return posY;
}

void Pacman::checkPelletCollision(vector<Pellet>& pellets) {
    for (int i = 0; i < pellets.size();) {
        SDL_Rect pelletRect = pellets[i].getRect();
        if (SDL_HasIntersection(&pacmanRect, &pelletRect)) {
            pellets.erase(pellets.begin() + i);
        }
        else {
            i++; 
        }
    }
}

SDL_Rect Pacman::getRect() {
    return pacmanRect;
}