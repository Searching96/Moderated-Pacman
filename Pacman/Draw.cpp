#include "Draw.h"

void drawPoint(SDL_Renderer* rd, int x, int y) {
    SDL_RenderDrawPoint(rd, x, y);
}

void drawCircle(SDL_Renderer* rd, float centerX, float centerY, int radius) {
    const int32_t diameter = (radius * 2);

    int32_t x = (radius - 1);
    int32_t y = 0;
    int32_t tx = 1;
    int32_t ty = 1;
    int32_t error = (tx - diameter);

    while (x >= y) {
        // 8-way symmetry
        drawPoint(rd, static_cast<int>(centerX) + x, static_cast<int>(centerY) - y);
        drawPoint(rd, static_cast<int>(centerX) - x, static_cast<int>(centerY) - y);
        drawPoint(rd, static_cast<int>(centerX) - x, static_cast<int>(centerY) + y);
        drawPoint(rd, static_cast<int>(centerX) + y, static_cast<int>(centerY) - x);
        drawPoint(rd, static_cast<int>(centerX) + x, static_cast<int>(centerY) + y);
        drawPoint(rd, static_cast<int>(centerX) + y, static_cast<int>(centerY) + x);
        drawPoint(rd, static_cast<int>(centerX) - y, static_cast<int>(centerY) - x);
        drawPoint(rd, static_cast<int>(centerX) - y, static_cast<int>(centerY) + x);

        if (error <= 0) {
            ++y;
            error += ty;
            ty += 2;
        }

        if (error > 0) {
            --x;
            tx += 2;
            error += (tx - diameter);
        }
    }
}

void drawTransparentCircle(SDL_Renderer* rd, float x, float y, int radius, SDL_Color color) {
    SDL_SetRenderDrawBlendMode(rd, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(rd, color.r, color.g, color.b, color.a);

    for (int w = 0; w < radius * 2; w++) {
        for (int h = 0; h < radius * 2; h++) {
            int dx = radius - w; // horizontal offset
            int dy = radius - h; // vertical offset
            if ((dx * dx + dy * dy) <= (radius * radius)) {
                SDL_RenderDrawPoint(rd, x + dx, y + dy);
            }
        }
    }
}

void drawGhostBody(SDL_Renderer* rd, int x, int y, int width, int height) {
    int centerX = x + width / 2;
    int centerY = y + height / 2;

    // Draw the semi-circle for the top part of the ghost
    for (int w = 0; w < width; ++w) {
        for (int h = 0; h < height / 2; ++h) {
            int dx = w - width / 2;
            int dy = h - height / 2;
            if ((dx * dx + dy * dy) <= (width / 2 * height / 2)) {
                SDL_RenderDrawPoint(rd, x + dx + width / 2, y + dy + height / 2);
            }
        }
    }

    // Draw the bottom part of the ghost
    SDL_Rect rect = { x, y + height / 2, width, height / 2 };
    SDL_RenderFillRect(rd, &rect);
}

void drawGhostEyes(SDL_Renderer* rd, int x, int y, int width, int height) {
    int eyeWidth = width / 5;
    int eyeHeight = height / 4;
    int eyeOffsetX = width / 4;
    int eyeOffsetY = height / 4;

    // Draw white part of the eyes
    SDL_SetRenderDrawColor(rd, 255, 255, 255, 255);
    SDL_Rect leftEye = { x + eyeOffsetX - eyeWidth / 2, y + eyeOffsetY - eyeHeight / 2, eyeWidth, eyeHeight };
    SDL_Rect rightEye = { x + width - eyeOffsetX - eyeWidth / 2, y + eyeOffsetY - eyeHeight / 2, eyeWidth, eyeHeight };
    SDL_RenderFillRect(rd, &leftEye);
    SDL_RenderFillRect(rd, &rightEye);

    // Draw the pupils
    SDL_SetRenderDrawColor(rd, 0, 0, 0, 255);
    int pupilWidth = eyeWidth / 2;
    int pupilHeight = eyeHeight / 2;
    SDL_Rect leftPupil = { x + eyeOffsetX - pupilWidth / 2, y + eyeOffsetY - pupilHeight / 2, pupilWidth, pupilHeight };
    SDL_Rect rightPupil = { x + width - eyeOffsetX - pupilWidth / 2, y + eyeOffsetY - pupilHeight / 2, pupilWidth, pupilHeight };
    SDL_RenderFillRect(rd, &leftPupil);
    SDL_RenderFillRect(rd, &rightPupil);
}

void drawFilledCircle(SDL_Renderer* renderer, int centerX, int centerY, int radius, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    for (int y = -radius; y <= radius; ++y) {
        for (int x = -radius; x <= radius; ++x) {
            if (x * x + y * y <= radius * radius) {
                SDL_RenderDrawPoint(renderer, centerX + x, centerY + y);
            }
        }
    }
}

void drawPacman(SDL_Renderer* renderer, int centerX, int centerY, int radius, int startAngle, int endAngle, SDL_Color bodyColor) {
    drawFilledCircle(renderer, centerX, centerY, radius, bodyColor);

    SDL_SetRenderDrawColor(renderer, 30, 30, 50, 255);

    for (int angle = startAngle; angle <= endAngle; ++angle) {
        float rad = angle * (M_PI / 180.0f);
        int x = centerX + static_cast<int>(radius * cos(rad));
        int y = centerY - static_cast<int>(radius * sin(rad));
        SDL_RenderDrawLine(renderer, centerX, centerY, x, y);
    }
}

void drawPellet(SDL_Renderer* renderer, int centerX, int centerY, int radius) {
    SDL_Color pelletColor = { 255, 165, 0, 255 };
    drawFilledCircle(renderer, centerX, centerY, radius, pelletColor);
}

void drawTile(SDL_Renderer* rd, int x, int y, int type) {
    SDL_Rect tileRect;
    tileRect.x = x;
    tileRect.y = y;
    tileRect.w = TILE_SIZE;
    tileRect.h = TILE_SIZE;

    switch (type) {
    case 0: 
        SDL_SetRenderDrawColor(rd, 30, 30, 50, 255); 
        break;
    case 1: 
        SDL_SetRenderDrawColor(rd, 0, 0, 255, 255); 
        break;
    default:
        SDL_SetRenderDrawColor(rd, 30, 30, 30, 255);
        break;
    }

    SDL_RenderFillRect(rd, &tileRect);

    if (type == 1) {
        SDL_SetRenderDrawColor(rd, 150, 150, 255, 255); 
        SDL_RenderDrawRect(rd, &tileRect); 
        tileRect.x += 1;
        tileRect.y += 1;
        tileRect.w -= 2;
        tileRect.h -= 2;
        SDL_RenderDrawRect(rd, &tileRect); 
        tileRect.x -= 1;
        tileRect.y -= 1;
        tileRect.w += 2;
        tileRect.h += 2;
    }
}