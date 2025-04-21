#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <vector>
using namespace std;
#include "Render.hpp"
#include "Entity.hpp"

// Funkcije za Renderer

Render::Render() {}

// Konstruktor, ustvari renderer pa naštima nastavitve ekrana
Render::Render(const char *p_title, int p_w, int p_h)

{
    SDL_DisplayMode dm;
    if (SDL_GetDesktopDisplayMode(0, &dm) != 0)
    {
        std::cout << "Failed to get display mode. Error: " << SDL_GetError() << std::endl;
        return;
    }

    int nativeWidth = dm.w;
    int nativeHeight = dm.h;

    std::cout << "Window size: " << nativeWidth << "x" << nativeHeight << std::endl;

    window = SDL_CreateWindow(p_title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, p_w, p_h, SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN);
    if (window == NULL)
    {
        std::cout << "Window failed to init. Error: " << SDL_GetError() << std::endl;
        return;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL)
    {
        std::cout << "Renderer failed to init. Error: " << SDL_GetError() << std::endl;
        return;
    }
}

// General cleanup ob zaključku igre
void Render::cleanUp()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    Mix_Quit();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

// Čistenje ekrana
void Render::clear()
{
    SDL_RenderClear(renderer);
}

// Prikaz
void Render::display()
{
    SDL_RenderPresent(renderer);
}

// Prikazovanje točnega dela teksture
void Render::renderTexture(SDL_Texture *p_tex, SDL_Rect srcRect, SDL_Rect dstRect)
{
    SDL_RenderCopy(renderer, p_tex, &srcRect, &dstRect);
}

// Prikazovajne cele teksture
void Render::renderTexture1(SDL_Texture *p_tex, SDL_Rect dstRect)
{
    SDL_RenderCopy(renderer, p_tex, NULL, &dstRect);
}

// Ustvarjanje teksture
SDL_Texture *Render::loadTexture(const char *p_filePath)
{
    SDL_Texture *texture = NULL;
    texture = IMG_LoadTexture(renderer, p_filePath);
    if (texture == NULL)
    {
        std::cout << "Failed to load texture. Error: " << IMG_GetError() << std::endl;
    }
    return texture;
}

// Prikazovanje entity, samo če je živ
void Render::renderEntity(Entity entity, int x, int y, int w, int h, bool alive)
{
    if (alive == false)
    {
        return;
    }
    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    dst.w = w;
    dst.h = h;

    SDL_RenderCopy(renderer, entity.getTex(), NULL, &dst);
}

// prikazovanje igralca
void Render::renderPlayer(Entity *player1)
{
    SDL_Rect dst;
    dst.x = 960;
    dst.y = 540;
    dst.w = 100;
    dst.h = 100;

    SDL_RenderCopy(renderer, player1->getTex(), NULL, &dst);
}

// Funkcije za tekst

// Konstruktor za Blended text
Text::Text(SDL_Renderer *renderer1, SDL_Color color1, TTF_Font *font1, string text1, int x1, int y1, int h1, int w1)
{
    renderer = renderer1;
    font = font1;
    this->text = text1;
    this->x = x1;
    this->y = y1;
    this->h = h1;
    this->w = w1;
    textColor = color1;
    renderQuad = {x - w, y, w, h};
    textSurface = TTF_RenderText_Blended(font, text.c_str(), textColor);
    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);
}

// Konstruktor za Shaded text
Text::Text(SDL_Renderer *renderer1, SDL_Color color1, TTF_Font *font1, string text1, int x1, int y1, int h1, int w1, SDL_Color color2)
{
    renderer = renderer1;
    font = font1;
    this->text = text1;
    this->x = x1;
    this->y = y1;
    this->h = h1;
    this->w = w1;
    textColor = color1;
    renderQuad = {x - w, y, w, h};
    textSurface = TTF_RenderText_Shaded(font, text.c_str(), textColor, color2);
    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);
}

// Prikazovanje teksta
void Text::renderText(int screenWidth, int screenHeight)
{
    renderQuad.x = screenWidth;
    renderQuad.y = screenHeight;
    SDL_RenderCopy(renderer, textTexture, NULL, &renderQuad);
}

// Prikazovanje teksta desno zgoraj
void Text::renderText1(int screenWidth, int screenHeight)
{
    renderQuad.x = screenWidth - renderQuad.w;
    renderQuad.y = 0;
    SDL_RenderCopy(renderer, textTexture, NULL, &renderQuad);
}

// Spreminjanje blended teksta
void Text::setText(string text1)
{
    text = text1;
    SDL_DestroyTexture(textTexture);
    textSurface = TTF_RenderText_Blended(font, text.c_str(), textColor);
    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);
}

// Spreminjanje shaded teksta
void Text::setText(string text1, SDL_Color color1)
{
    text = text1;
    SDL_DestroyTexture(textTexture);
    textSurface = TTF_RenderText_Shaded(font, text.c_str(), textColor, color1);
    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);
}

// Spreminjanje shaded_wrapped teksta
void Text::setText(string text1, int blend)
{
    text = text1;
    SDL_DestroyTexture(textTexture);
    textSurface = TTF_RenderText_Shaded_Wrapped(font, text.c_str(), textColor, {0, 0, 0, 255}, blend);
    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);
}

// Konstruktor za shaded_wrapped tekst
Text::Text(SDL_Renderer *renderer1, SDL_Color color1, TTF_Font *font1, string text1, int x1, int y1, int h1, int w1, int blend)
{
    renderer = renderer1;
    font = font1;
    this->text = text1;
    this->x = x1;
    this->y = y1;
    this->h = h1;
    this->w = w1;
    textColor = color1;
    renderQuad = {x - w, y, w, h};
    textSurface = TTF_RenderText_Shaded_Wrapped(font, text.c_str(), textColor, {176, 124, 1, 255}, blend);
    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);
}
