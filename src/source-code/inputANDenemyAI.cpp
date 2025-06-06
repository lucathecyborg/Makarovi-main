#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Entity.hpp"
#include "Player.hpp"
#include "inputANDenemyAI.hpp"
#include "menu.hpp"
#include "Render.hpp"
#include "levels.hpp"
#include <vector>
#include <iostream>

// funkcija za enemyAI, nasprotniki zasledujejo igralcu če so okoli njega v krogu s radijem 750
void enemyAI(std::vector<Entity> &enemies, int number, Level levels[], int level_counter, int modifier)
{

    float movement = 0;
    if (modifier == 1)
        movement = 0.5;
    else if (modifier == 3)
        movement = 0.8;
    else if (modifier == 4)
        movement = 1.2;
    SDL_Rect *tempRect;
    for (int i = 0; i < number; i++)
    {
        // pogleda če je enemy v krogu
        if (enemies[i].Circle(enemies[i].getX(), enemies[i].getY(), 750))
        {
            // dobi pozicijo nasprotnika odvisno od igralca
            CirclePos pos = enemies[i].CheckLocation(enemies[i].getX(), enemies[i].getY(), 750);
            if (enemies[i].getY() > 540 - 10 && enemies[i].getY() < 540 + 10)
            {
            }

            tempRect = enemies[i].getHitbox();

            if (pos.Above)
            {
                tempRect->y += movement;
                if (!levels[level_counter].enemyCheckCollision(enemies[i].getHitbox()))
                {
                    enemies[i].Move(enemies[i].getX(), enemies[i].getY() + movement);
                }
            }

            tempRect = enemies[i].getHitbox();

            if (pos.Below)
            {
                tempRect->y -= movement;
                if (!levels[level_counter].enemyCheckCollision(enemies[i].getHitbox()))
                {
                    enemies[i].Move(enemies[i].getX(), enemies[i].getY() - movement);
                }
            }

            if (enemies[i].getX() > 940 - 10 && enemies[i].getX() < 940 + 5)
            {
            }

            tempRect = enemies[i].getHitbox();

            if (pos.Right)
            {
                tempRect->x -= movement;
                if (!levels[level_counter].enemyCheckCollision(enemies[i].getHitbox()))
                {
                    enemies[i].Move(enemies[i].getX() - movement, enemies[i].getY());
                }
            }

            tempRect = enemies[i].getHitbox();

            if (pos.Left)
            {
                tempRect->x += movement;
                if (!levels[level_counter].enemyCheckCollision(enemies[i].getHitbox()))
                {
                    enemies[i].Move(enemies[i].getX() + movement, enemies[i].getY());
                }
            }
        }
    }
}

// Za preverjanje vseh vnosov igralca
bool inputHandling(SDL_Event event, bool &gameRunning, Player &player1, SDL_Texture *player_Walking_Forward[], SDL_Texture *player_Walking_Backward[], bool &punch, Render &window, bool &moveUp, bool &moveDown, bool &moveLeft, bool &moveRight, Level level[], int &level_counter, vector<char> movement)
{
    bool input = false;
    const Uint8 *keystates = SDL_GetKeyboardState(NULL);
    moveUp = keystates[SDL_SCANCODE_W] || keystates[SDL_SCANCODE_UP];
    moveDown = keystates[SDL_SCANCODE_S] || keystates[SDL_SCANCODE_DOWN];
    moveLeft = keystates[SDL_SCANCODE_A] || keystates[SDL_SCANCODE_LEFT];
    moveRight = keystates[SDL_SCANCODE_D] || keystates[SDL_SCANCODE_RIGHT];

    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            gameRunning = false;
        }
        if (event.type == SDL_KEYDOWN)
        {
            input = true;
            // udarec
            if (keystates[SDL_SCANCODE_SPACE])
            {
                punch = true;
            }
            // gledanje rešenih ugank
            if (event.key.keysym.sym == SDLK_TAB)
            {
                level[level_counter].clueRoom(window);
            }
        }

        if (event.type == SDL_KEYUP)
        {
            // pavziranje
            if (event.key.keysym.sym == SDLK_ESCAPE)
            {
                Mix_PauseMusic();
                level_counter = menu(window, level, level_counter, movement); // Update level_counter with the returned value
                Mix_ResumeMusic();
            }
            // resetiranje slike igralca
            if (event.key.keysym.sym == SDLK_UP)
            {
                player1.setTex(player_Walking_Backward[0]);
            }
            else if (event.key.keysym.sym == SDLK_DOWN)
            {
                player1.setTex(player_Walking_Forward[0]);
            }
        }
    }

    return input;
}

// Prvi setup ki se zgodi ob zagonu levela (animacija)
bool playerSetup(Entity &player, SDL_Texture *mapTex, Render window, SDL_Rect srcRect, SDL_Texture *player_walking_backwards[], Level &currentLevel, int modifyer)
{
    // prikaže startup ekran
    currentLevel.start();
    int currentTicks = 0;
    int x = 960, y = 1180;
    int i = 0;
    player.Move(x, y);
    // počasi premika igralca gor po ekranu
    while (y != 540)
    {
        int Ticks = SDL_GetTicks();
        if (Ticks - currentTicks > 200)
        {
            if (i > 3)
            {
                i = 0;
            }
            currentTicks = Ticks;
            player.setTex(player_walking_backwards[i]);
            i++;
        }

        window.clear();
        window.renderTexture(mapTex, srcRect, {0, 0, 1920, 1080});
        window.renderEntity(player, x, y, 100, 100, true);
        window.display();
        if (y <= 540)
        {
            return true;
        }
        else
        {
            y -= modifyer + 1;
        }
    }
    return false;
}

// Funkija za prikaz teksta win/lose
void gameOver(Render window, TTF_Font *font, bool win)
{
    if (win)
    {
        SDL_SetRenderDrawColor(window.getRenderer(), 0, 0, 0, 255);
        window.clear();
        Text GameOvertText(window.getRenderer(), {0, 255, 0, 255}, font, "YOU WIN", 500, 500, 800, 1200);
        GameOvertText.renderText1(1560, 540);
        window.display();
        SDL_Delay(2000);
    }
    else
    {
        SDL_SetRenderDrawColor(window.getRenderer(), 0, 0, 0, 255);
        window.clear();
        Text GameOvertText(window.getRenderer(), {255, 0, 0, 255}, font, "GAME OVER", 500, 500, 800, 1200);
        GameOvertText.renderText1(1560, 540);
        window.display();
        SDL_Delay(2000);
    }
}

// Quit funkcija
void Quit(Render window)
{
    window.cleanUp();
    exit(0);
}

/* Template funkcija za pisanja teksta s uporaba SDL_StartTextInput()

void writeText(Render window)
{
    bool text = true;
    SDL_Event e;
    SDL_StartTextInput();
    string input = "";
    while (text)
    {

        while (SDL_PollEvent(&e))
        {
            switch (e.type)
            {
            case SDL_QUIT:
                text = false;
                Quit(window);
                break;

            case SDL_KEYDOWN:
                if (e.key.keysym.sym == SDLK_BACKSPACE && !input.empty())
                {
                    input.pop_back();
                }
                else if (e.key.keysym.sym == SDLK_RETURN)
                    text = false;
                break;
            case SDL_TEXTINPUT:
                input += e.text.text;
                break;
            }
            std::cout << input << std::endl;
        }
    }
    SDL_StopTextInput();
} */