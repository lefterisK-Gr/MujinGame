#pragma once

#include "ECS\ECS.h"
#include <TTF/SDL_ttf.h>
#include "Game.h"

class ShapeManager {
public:
    static void DrawCircle(int32_t centreX, int32_t centreY, int32_t radius, SDL_Color color)
    {
        SDL_SetRenderDrawColor(Game::renderer, color.r, color.g, color.b, color.a);
        for (int w = 0; w < radius * 2; w++)
        {
            for (int h = 0; h < radius * 2; h++)
            {
                int dx = radius - w; // horizontal offset
                int dy = radius - h; // vertical offset
                if ((dx * dx + dy * dy) <= (radius * radius))
                {
                    SDL_RenderDrawPoint(Game::renderer, centreX + dx, centreY + dy);
                }
            }
        }
    }
};
