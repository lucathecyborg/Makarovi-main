#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>

#include "Render.hpp"
#include "Entity.hpp"
#include "Player.hpp"
#include "button.hpp"

Button::Button(int x, int y, int w1, int h1, SDL_Texture *tex1)
{
    tex = tex1;
    hitbox = {x, y, w1, h1};
}
Button::Button(int x, int y, int w1, int h1, SDL_Texture *tex1, SDL_Texture *tex2)
{
    tex = tex1;
    pressedTex = tex2;
    hitbox = {x, y, w1, h1};
}

void Button::setPressed(bool status)
{
    pressed = status;
}

bool Button::GetPressed()
{
    return pressed;
}

bool Button::IsPressed()
{
    return pressed;
}

void Button::render(Render &window, SDL_Rect mouseRect)
{
    if (SDL_HasIntersection(&mouseRect, &hitbox))
        window.renderTexture1(pressedTex, hitbox);
    else
        window.renderTexture1(tex, hitbox);
}

void Button::setPosition(int x, int y, int w1, int h1)
{
    hitbox = {x, y, w1, h1};
}

SDL_Rect *Button::getHitbox()
{
    return &hitbox;
}
