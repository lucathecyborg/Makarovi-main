#pragma once
#ifndef RENDER_HPP
#define RENDER_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string>
using namespace std;
class Entity; // Forward declare the Entity classs

class Render
{
public:
    Render(const char *p_title, int p_w, int p_h);
    Render();
    SDL_Texture *loadTexture(const char *p_filePath);
    void cleanUp();
    void clear();
    void display();
    void renderPlayer(Entity *player1);
    void renderTexture(SDL_Texture *p_tex, SDL_Rect srcRect, SDL_Rect dstRect);
    void renderTexture1(SDL_Texture *p_tex, SDL_Rect dstRect);
    void renderEntity(Entity entity, int x, int y, int w, int h, bool alive); // Use forward declared Entity class
    SDL_Renderer *getRenderer() { return renderer; };
    SDL_Window *getWindow() { return window; };

private:
    SDL_Window *window;
    SDL_Renderer *renderer;
};

class Text
{
    SDL_Color textColor;
    SDL_Surface *textSurface;
    SDL_Texture *textTexture;
    TTF_Font *font;
    SDL_Rect renderQuad;
    SDL_Renderer *renderer;
    int x, y, w, h;
    string text;

public:
    Text() {};
    void setText(string text1);
    void setText(string text1, int blend);
    void setText(string text1, SDL_Color color1);
    Text(SDL_Renderer *renderer, SDL_Color color, TTF_Font *font, string text, int x, int y, int h, int w);
    Text(SDL_Renderer *renderer, SDL_Color color, TTF_Font *font, string text, int x, int y, int h, int w, int blend);
    Text(SDL_Renderer *renderer1, SDL_Color color1, TTF_Font *font1, string text1, int x1, int y1, int h1, int w1, SDL_Color color2);
    void renderText(int screenWidth, int screenHeight);
    void renderText1(int screenWidth, int screenHeight);
};

#endif
