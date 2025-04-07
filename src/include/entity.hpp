#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <fstream>
#include "Render.hpp"
#include "levels.hpp"

class Entity
{
public:
    Entity(float p_x, float p_y, SDL_Texture *p_tex, bool life);
    Entity();
    float const getX();
    float const getY();
    SDL_Texture *getTex();
    void setTex(SDL_Texture *p_tex);
    void Move(float x, float y);
    bool Circle(float X, float Y, int radius);
    struct CirclePos CheckLocation(float X, float Y, int radius);
    SDL_Rect *getHitbox(); // Mark as const for read-only access
    int getHBY();
    int getHBX();
    void setAlive(bool status);
    bool const Alive(); // Mark as const for read-only access
    float const getPrevX();
    float const getPrevY();
    void setPrevX(float x1);
    void setPrevY(float y1);

protected:
    float prevX, prevY;
    float x, y;
    SDL_Texture *tex;
    SDL_Rect hitbox;
    bool alive;
};

struct CirclePos
{
    bool Above;
    bool Below;
    bool Right;
    bool Left;
};

class Clue : public Entity
{
private:
    bool solved = false;
    string clueType = "";
    string question = "";
    string answer = "";
    string odgovor = "";
    string clueText = "";
    Text clue;
    Text answers;
    TTF_Font *font = TTF_OpenFont("src/res/dev/IBMPlexMono-Medium.ttf", 70);
    TTF_Font *answerFont = TTF_OpenFont("src/res/dev/IBMPlexMono-Medium.ttf", 70);
    SDL_Texture *answerTex;
    SDL_Color textColor = {0, 0, 0, 255};
    Render window1;
    string tempAns;

public:
    Clue() {};
    Clue(float p_x, float p_y, bool life, Render &window);
    void SetClueType(string type, string text);
    void SetClueType(string type, string question, string odgovor1, string text);
    void solve(Level &currentLevel);
    bool const getSolved();
    string getType() { return clueType; };
    void displayClueText(Render &window, string text);
    string getAnswer();
};