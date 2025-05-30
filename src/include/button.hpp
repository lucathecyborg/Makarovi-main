#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include "Render.hpp"

class Button
{
    SDL_Rect hitbox;
    int w, h;
    SDL_Texture *tex;
    SDL_Texture *pressedTex;

public:
    Button(int x, int y, int w1, int h1, SDL_Texture *tex1, SDL_Texture *tex2);
    SDL_Rect *getHitbox();
    void render(Render &window, SDL_Rect mouseRect);
    void setPosition(int x, int y, int w, int h);
    SDL_Texture *getTex1()
    {
        return tex;
    };
    SDL_Texture *getTex2()
    {
        return pressedTex;
    };
};