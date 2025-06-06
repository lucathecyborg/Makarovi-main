#include "Entity.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <fstream>

#include "menu.hpp"
#include "inputANDenemyAI.hpp"
#include "levels.hpp"

// Funkcije za entities

float const Entity::getX()
{
    return x;
}

float const Entity::getY()
{
    return y;
}

int Entity::getHBY()
{
    return this->hitbox.y;
}

int Entity::getHBX()
{
    return this->hitbox.x;
}

SDL_Texture *Entity::getTex()
{
    return tex;
}

// Konstruktor
Entity::Entity(float p_x, float p_y, SDL_Texture *p_tex, bool life)
    : x(p_x), y(p_y), tex(p_tex)
{
    alive = life;
    hitbox = {static_cast<int>(p_x), static_cast<int>(p_y), 100, 100};
}

void Entity::setTex(SDL_Texture *p_tex)
{
    tex = p_tex;
}

// Default konstruktor
Entity::Entity() {};

// Premakne entity na podano mesto
void Entity::Move(float x1, float y1)
{
    prevX = x;
    prevY = y;
    this->x = x1;
    this->y = y1;
    this->hitbox.x = static_cast<int>(x1);
    this->hitbox.y = static_cast<int>(y1);
}

// Vrne true/false če je entity s podanim X in Y v krogu s polmerom "radius" okoli igralca
bool Entity::Circle(float X, float Y, int radius)
{
    int DeltaX = X - 940;
    int DeltaY = Y - 540;
    int SqaureDistance = DeltaX * DeltaX + DeltaY * DeltaY;
    return SqaureDistance <= radius * radius;
}

// Vrne struct CirclePos (pove pozicijo entity odvisno od igralca), če je entity s podanimi X in Y v krogu okoli igralca.
// Uporabljen skupaj s Entity::Circle za enemyAI
CirclePos Entity::CheckLocation(float X, float Y, int radi)
{
    CirclePos pos = {false, false, false, false};
    if (Circle(X, Y, radi))
    {
        if (Y > 540)
        {

            pos.Below = true;
        }
        else
        {
            pos.Above = true;
        }
        if (X > 940)
        {

            pos.Right = true;
        }
        else
        {
            pos.Left = true;
        }
    }
    return pos;
}

SDL_Rect *Entity::getHitbox()
{
    return &hitbox;
}

bool const Entity::Alive()
{
    return alive;
}

void Entity::setAlive(bool status)
{
    alive = status;
}

// Funkcije za uganke

// Konstruktor
Clue::Clue(float p_x, float p_y, bool life, Render &window)
{
    x = p_x;
    y = p_y;
    alive = life;
    window1 = window;
    answerTex = window1.loadTexture("src/res/gfx/answer.png");
    tex = window1.loadTexture("src/res/gfx/clue.png");
    hitbox = {static_cast<int>(p_x), static_cast<int>(p_y), 100, 100};
}

bool const Clue::getSolved()
{
    return solved;
}

// Reševanje uganke, glede na tip (uganka ali uprašanje)
void Clue::solve(Level &currentLevel)
{
    SDL_Event event;
    bool solveRunning = true;
    bool text = false;
    std::cout << clueType << std::endl;
    Mix_PlayChannel(-1, clueSound, 0);
    while (solveRunning)
    {
        // Pokaže uganko dokler ne pritisneš enter
        if (clueType == "clue")
        {
            clue.renderText(0, 0);

            window1.display();

            while (SDL_PollEvent(&event))
            {
                if (event.type == SDL_KEYDOWN)
                {
                    if (event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_SPACE)
                    {
                        solved = true;
                        goto end;
                    }
                }
            }
        }
        else if (clueType == "question")
        {
            // Prikazuje uprašanje ter omogoča vpis odgovora
            clue.renderText(0, 0);
            window1.renderTexture1(answerTex, {640, 799, 700, 220});
            text = true;
            SDL_StartTextInput();
            window1.display();

        input:
            while (text)
            {
                while (SDL_PollEvent(&event))
                {
                    switch (event.type)
                    {
                    case SDL_KEYDOWN:
                        if (event.key.keysym.sym == SDLK_BACKSPACE && !answer.empty())
                        {
                            answer.pop_back();
                        }
                        break;
                    case SDL_KEYUP:
                        if (event.key.keysym.sym == SDLK_ESCAPE)
                        {
                            goto end;
                        }
                        else if (event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_SPACE)
                        {
                            text = false;
                        }
                        break;
                    case SDL_TEXTINPUT:
                        if (answer.size() < 19)
                        {
                            answer += event.text.text;
                        }
                        break;
                    }
                }
                window1.clear();
                currentLevel.render();
                clue.renderText(0, 0);
                answers.setText(answer);
                window1.renderTexture1(answerTex, {640, 799, 700, 220});
                answers.renderText(673, 858);
                window1.display();
            }

            if (answer != odgovor)
            {
                // Če napačno uganeš ti izbriše odgovor in resetira
                answer.clear();
                text = true;
                goto input;
            }
            else if (answer == odgovor)
            {
                // Če pravilno uganeš ti pokaže nasljedjo uganko ter zaigra zvok
                text = false;
                solveRunning = false;
                solved = true;
                answer.clear();
                window1.clear();
                currentLevel.render();
                Mix_PlayChannel(-1, clueSound, 0);
                displayClueText(window1, clueText);
            }
            else
            {
            end:
                text = false;
                solveRunning = false;
            }
        }
    }
}

// Prikaže uganko, dokler ne pritisneš enter ali space
// uporabljen za uganke tipa uprašaja
void Clue::displayClueText(Render &window, string text)
{
    Text clue1(window.getRenderer(), textColor, answerFont, text, 0, 0, 300, 900, 900);
    clue1.renderText(0, 0);
    window.display();
    bool running = true;
    SDL_Event event;
    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_SPACE)
                {
                    running = false;
                }
            }
        }
    }
}

// Naredi uganko tipa uganka
void Clue::SetClueType(string type, string text)
{
    clueType = type;
    tempAns = text;
    Text clue1(window1.getRenderer(), textColor, font, text, 0, 0, 200, 900, 900);
    clue = clue1;
}

// Naredi ugano tipa uprašanje, dopiše še pravilni odgovor ter dejansko uganko
void Clue::SetClueType(string type, string question, string odgovor1, string text)
{
    odgovor = odgovor1;
    clueText = text;
    clueType = type;
    Text clue1(window1.getRenderer(), textColor, font, question, 0, 0, 300, 900, 900);
    clue = clue1;
    Text answer1(window1.getRenderer(), textColor, answerFont, answer, 673, 858, 104, 618);
    answers = answer1;
    tempAns = text;
}

string Clue::getAnswer()
{
    return tempAns;
}
