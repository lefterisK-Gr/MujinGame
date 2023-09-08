#pragma once

#include "ECS\ECS.h"
#include "SDL_ttf.h"

void DrawCircle(SDL_Renderer* renderer, int32_t centreX, int32_t centreY, int32_t radius)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 0);
    for (int w = 0; w < radius * 2; w++)
    {
        for (int h = 0; h < radius * 2; h++)
        {
            int dx = radius - w; // horizontal offset
            int dy = radius - h; // vertical offset
            if ((dx * dx + dy * dy) <= (radius * radius))
            {
                SDL_RenderDrawPoint(renderer, centreX + dx, centreY + dy);
            }
        }
    }
}