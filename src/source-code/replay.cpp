#include "Entity.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>
#include <fstream>

#include "levels.hpp"
void saveReplay(Level &level, vector<char> movement)
{
    string filename = std::to_string(static_cast<char>(level.getLevelNumber()));
    ofstream data2(filename + "replay.txt");
    for (int i = 0; i < movement.size(); i++)
    {
        data2 << movement[i];
    }

    data2.close();
}

void playReplay(Level &level, vector<char> movement, Render &window)
{

    SDL_Rect tempRect = {(level.getX() - 1920) / 2, (level.getY() - 1080) / 2, 1920, 1080};
    SDL_Texture *tempMap = level.getMap();
    string filename = std::to_string(static_cast<char>(level.getLevelNumber()));

    ifstream data(filename + "replay.txt");
    int speed = level.getSpeed();
    if (!data.is_open())
    {
        std::cerr << "Failed to open replay file!" << std::endl;
        cout << filename << endl;
        return;
    }
    uint32_t startTicks = SDL_GetTicks();
    Text text1(window.getRenderer(), {0, 0, 0, 255}, TTF_OpenFont("src/res/dev/IBMPlexMono-Medium.ttf", 100), "Time: " + std::to_string(0), 100, 100, 100, 300, {255, 255, 255, 75});

    char move;
    SDL_Event event;
    bool running = true;

    while (running)
    {
        uint32_t elapsedTicks = (SDL_GetTicks() - startTicks) / 1000;

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_KEYUP && (event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_SPACE || event.key.keysym.sym == SDLK_ESCAPE))
            {
                running = false;
                break;
            }
        }

        if (data >> noskipws >> move)
        {

            if (move == 'u')
            {
                tempRect.y -= speed;
            }
            else if (move == 'd')
            {
                tempRect.y += speed;
            }
            else if (move == 'l')
            {
                tempRect.x -= speed;
            }
            else if (move == 'r')
            {
                tempRect.x += speed;
            }
            else if (move == ' ')
            {
            }

            else if (move == 'x' || move == 'n' || move == 'w')
            {
                running = false;
                break;
            }

            window.clear();
            window.renderTexture(tempMap, tempRect, {0, 0, 1920, 1080});
            window.renderTexture1(window.loadTexture("src/res/gfx/ppl_textures/player/moving forward/moving f1.png"), {960, 540, 100, 100});
            text1.setText("Time: " + std::to_string(elapsedTicks) + "s", {255, 255, 255, 75});
            text1.renderText(0, 0);
            window.display();
        }
        else
        {
            running = false;
        }
    }

    data.close();
}