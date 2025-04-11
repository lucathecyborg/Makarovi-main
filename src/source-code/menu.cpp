#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>
#include <ctime>
#include <string>
#include <fstream>

#include "inputANDenemyAI.hpp"
#include "Render.hpp"
#include "Entity.hpp"
#include "Player.hpp"
#include "button.hpp"
#include "menu.hpp"

int menu(Render &window, Level levels[], int &levelNumber)
{

    bool menu = true;
    SDL_Event event;
    SDL_Texture *menuTex = window.loadTexture("src/res/dev/main menu.png");
    SDL_Texture *saveTex = window.loadTexture("src/res/dev/save pressed.png");
    SDL_Texture *loadTex = window.loadTexture("src/res/dev/load pressed.png");
    SDL_Texture *playTex = window.loadTexture("src/res/dev/play pressed.png");
    SDL_Texture *creditsTex = window.loadTexture("src/res/dev/credits pressed.png");
    SDL_Texture *optionsTex = window.loadTexture("src/res/dev/options pressed.png");
    SDL_Texture *quitTex = window.loadTexture("src/res/dev/quit pressed.png");
    SDL_Texture *tutorialTex = window.loadTexture("src/res/dev/tutorial/Tutorial.png");
    SDL_Texture *tutorialTexPressed = window.loadTexture("src/res/dev/tutorial/tutorial_pressed.png");
    Button start(771, 380, 378, 86, playTex);
    Button quit(771, 850, 378, 86, quitTex);
    Button creditsButton(771, 692, 378, 86, creditsTex);
    Button optionsButton(771, 540, 378, 86, optionsTex);
    Button saveButon(177, 540, 378, 86, saveTex);
    Button loadButon(177, 727, 378, 86, loadTex);
    Button tutbutton(1364, 540, 378, 86, tutorialTex, tutorialTexPressed);
    Button clueButton(1364, 727, 378, 86, window.loadTexture("src/res/dev/clue.png"), window.loadTexture("src/res/dev/clue pressed.png"));
    while (menu)
    {
        window.renderTexture(menuTex, {0, 0, 1920, 1080}, {0, 0, 1920, 1080});
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        SDL_Rect mouseRect = {mouseX, mouseY, 1, 1};
        if (SDL_HasIntersection(&mouseRect, start.getHitbox()))
        {
            window.renderTexture(playTex, {0, 0, 378, 86}, {771, 380, 378, 86});
        }
        else if (SDL_HasIntersection(&mouseRect, quit.getHitbox()))
        {
            window.renderTexture(quitTex, {0, 0, 378, 86}, {771, 850, 378, 86});
        }
        else if (SDL_HasIntersection(&mouseRect, creditsButton.getHitbox()))
        {
            window.renderTexture(creditsTex, {0, 0, 378, 86}, {771, 692, 378, 86});
        }
        else if (SDL_HasIntersection(&mouseRect, optionsButton.getHitbox()))
        {
            window.renderTexture(optionsTex, {0, 0, 378, 86}, {771, 540, 378, 86});
        }
        else if (SDL_HasIntersection(&mouseRect, saveButon.getHitbox()))
        {
            window.renderTexture(saveTex, {0, 0, 378, 86}, {177, 540, 378, 86});
        }
        else if (SDL_HasIntersection(&mouseRect, loadButon.getHitbox()))
        {
            window.renderTexture(loadTex, {0, 0, 378, 86}, {177, 727, 378, 86});
        }
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                if (event.button.button == SDL_BUTTON_LEFT)
                {

                    if (SDL_HasIntersection(&mouseRect, start.getHitbox()))
                    {
                        menu = false;
                    }
                    else if (SDL_HasIntersection(&mouseRect, quit.getHitbox()))
                    {
                        menu = false;
                        Quit(window);
                        return levelNumber;
                    }
                    else if (SDL_HasIntersection(&mouseRect, creditsButton.getHitbox()))
                    {
                        credits(window);
                    }
                    else if (SDL_HasIntersection(&mouseRect, optionsButton.getHitbox()))
                    {
                        options(window);
                    }
                    else if (SDL_HasIntersection(&mouseRect, saveButon.getHitbox()))
                    {
                        levels[levelNumber].saveToFile();
                        TTF_Font *Savefont = TTF_OpenFont("src/res/dev/IBMPlexMono-Medium.ttf", 140);
                        Text saveText(window.getRenderer(), {255, 0, 0, 255}, Savefont, "Game Saved", 500, 500, 400, 1000, {0, 0, 0, 255});
                        saveText.renderText(450, 300);
                        window.display();
                        SDL_Delay(2000);
                    }
                    else if (SDL_HasIntersection(&mouseRect, tutbutton.getHitbox()))
                    {
                        tutorial(window);
                    }

                    else if (SDL_HasIntersection(&mouseRect, loadButon.getHitbox()))
                    {
                        levelNumber = levels[levelNumber].numberCheck() - 1;
                        levels[levelNumber].loadFromFile(window);
                        TTF_Font *Loadfont = TTF_OpenFont("src/res/dev/IBMPlexMono-Medium.ttf", 140);
                        Text saveText(window.getRenderer(), {255, 0, 0, 255}, Loadfont, "Game loaded", 500, 500, 400, 1000, {0, 0, 0, 255});
                        saveText.renderText(450, 300);
                        window.display();
                        SDL_Delay(2000);
                    }
                    else if (SDL_HasIntersection(&mouseRect, clueButton.getHitbox()))
                    {
                        levels[levelNumber].clueRoom(window);
                    }
                }
            }
            if (event.type == SDL_QUIT)
            {

                menu = false;
                Quit(window);
                return levelNumber;
            }
            if (event.type == SDL_KEYUP)
            {
                if (event.key.keysym.sym == SDLK_ESCAPE)
                {
                    menu = false;
                }
            }
        }
        clueButton.render(window, mouseRect);
        tutbutton.render(window, mouseRect);
        window.display();
        window.clear();
    }

    return levelNumber;
}

void credits(Render &window)
{

    bool credits = true;
    SDL_Event event;
    SDL_Texture *quitTex = window.loadTexture("src/res/dev/quit pressed3.png");
    Button back(1507, 960, 378, 86, quitTex);
    SDL_Texture *creditsTex = window.loadTexture("src/res/dev/credits.png");
    while (credits)
    {
        window.renderTexture(creditsTex, {0, 0, 1920, 1080}, {0, 0, 1920, 1080});
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        SDL_Rect mouseRect = {mouseX, mouseY, 1, 1};
        if (SDL_HasIntersection(back.getHitbox(), &mouseRect))
        {
            window.renderTexture(quitTex, {0, 0, 378, 86}, {1507, 960, 378, 86});
        }

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_KEYUP)
            {
                if (event.button.button == SDL_BUTTON_LEFT)
                {

                    if (SDL_HasIntersection(&mouseRect, back.getHitbox()))
                    {
                        credits = false;
                    }
                }
                else if (event.key.keysym.sym == SDLK_ESCAPE)
                {
                    credits = false;
                }
            }
            if (event.type == SDL_QUIT)
            {
                credits = false;
            }
        }

        window.display();
        window.clear();
    }
}

void options(Render &window)
{
    bool options = true;
    SDL_Event event;
    float brightness = 0;
    SDL_Texture *buttonTex = window.loadTexture("src/res/dev/quit pressed2.png");
    Button back(1506, 979, 378, 86, buttonTex);
    SDL_Texture *optionsTex = window.loadTexture("src/res/dev/options.png");

    while (options)
    {
        window.renderTexture(optionsTex, {0, 0, 1920, 1080}, {0, 0, 1920, 1080});
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        SDL_Rect mouseRect = {mouseX, mouseY, 1, 1};

        if (SDL_HasIntersection(back.getHitbox(), &mouseRect))
        {
            window.renderTexture(buttonTex, {0, 0, 378, 86}, {1506, 979, 378, 86});
        }

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_KEYUP)
            {
                if (event.button.button == SDL_BUTTON_LEFT)
                {

                    if (SDL_HasIntersection(&mouseRect, back.getHitbox()))
                    {
                        options = false;
                    }
                }
                if (event.key.keysym.sym == SDLK_ESCAPE)
                {
                    options = false;
                }
            }
            if (event.type == SDL_QUIT)
            {
                options = false;
            }

            if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.sym == SDLK_UP)
                {
                    brightness -= 0.1;
                    SDL_SetWindowBrightness(window.getWindow(), 1 - brightness);
                }
                if (event.key.keysym.sym == SDLK_DOWN)
                {
                    brightness += 0.1;
                    SDL_SetWindowBrightness(window.getWindow(), 1 - brightness);
                }
                if (event.key.keysym.sym == SDLK_RETURN)
                {
                    brightness = 0;
                    SDL_SetWindowBrightness(window.getWindow(), 1);
                }
                if (event.key.keysym.sym == SDLK_LEFT)
                {
                    Mix_VolumeMusic(Mix_VolumeMusic(-1) - 10);
                }
                if (event.key.keysym.sym == SDLK_RIGHT)
                {
                    Mix_VolumeMusic(Mix_VolumeMusic(-1) + 10);
                }
            }
        }

        window.display();
        window.clear();
    }
}

void tutorial(Render &window)
{
    window.clear();
    SDL_Texture *pages[6];
    pages[0] = window.loadTexture("src/res/dev/tutorial/1.png");
    pages[1] = window.loadTexture("src/res/dev/tutorial/2.png");
    pages[2] = window.loadTexture("src/res/dev/tutorial/3.png");
    pages[3] = window.loadTexture("src/res/dev/tutorial/4.png");
    pages[4] = window.loadTexture("src/res/dev/tutorial/5.png");
    pages[5] = window.loadTexture("src/res/dev/tutorial/6.png");

    SDL_Texture *arrowB = window.loadTexture("src/res/dev/tutorial/arrow2.png");
    SDL_Texture *arrowF = window.loadTexture("src/res/dev/tutorial/arrow.png");
    SDL_Texture *arrowFP = window.loadTexture("src/res/dev/tutorial/arrowP.png");
    SDL_Texture *arrowBP = window.loadTexture("src/res/dev/tutorial/arrow2P.png");

    Button backButton(0, 532, 240, 132, arrowB, arrowBP);
    Button frontButton(1680, 532, 240, 132, arrowF, arrowFP);
    SDL_Texture *buttonTex = window.loadTexture("src/res/dev/quit pressed2.png");
    SDL_Texture *quitTex = window.loadTexture("src/res/dev/tutorial/quit.png");
    Button back(50, 979, 378, 86, quitTex, buttonTex);
    bool tut = true;
    int page = 0;
    SDL_Event event;

    while (tut)
    {
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        SDL_Rect mouseRect = {mouseX, mouseY, 1, 1};
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_MOUSEBUTTONUP)
            {
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    if (SDL_HasIntersection(&mouseRect, backButton.getHitbox()))
                    {
                        if (page > 0)
                        {
                            page--;
                        }
                    }
                    else if (SDL_HasIntersection(&mouseRect, frontButton.getHitbox()))
                    {
                        if (page < 5)
                        {
                            page++;
                        }
                    }
                    else if (SDL_HasIntersection(&mouseRect, back.getHitbox()))
                    {
                        tut = false;
                    }
                }
            }
        }
        window.clear();
        window.renderTexture1(pages[page], {0, 0, 1920, 1080});
        backButton.render(window, mouseRect);
        frontButton.render(window, mouseRect);
        back.render(window, mouseRect);
        window.display();
    }
}

int selectScreen(Render &window, bool win)
{
    Button replayFromStart(150, 546, 410, 200, window.loadTexture("src/res/dev/replayStart.png"), window.loadTexture("src/res/dev/replayStart pressed.png"));
    Button replayLevel(1360, 540, 410, 200, window.loadTexture("src/res/dev/replayLevel.png"), window.loadTexture("src/res/dev/replayLevel pressed.png"));
    Button quitAndSave(494, 825, 410, 200, window.loadTexture("src/res/dev/save and quit.png"), window.loadTexture("src/res/dev/save and quit pressed.png"));
    Button quit(1010, 825, 410, 200, window.loadTexture("src/res/dev/quit without saving.png"), window.loadTexture("src/res/dev/quit without saving pressed.png"));
    SDL_Texture *backroundWin = window.loadTexture("src/res/dev/endBackroundWin.png");
    SDL_Texture *image;
    if (win)
        image = window.loadTexture("src/res/dev/win.png");
    else
        image = window.loadTexture("src/res/dev/lose.png");

    bool running = true;
    SDL_Event event;
    while (running)
    {
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        SDL_Rect mouseRect = {mouseX, mouseY, 1, 1};
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
            {
                if (SDL_HasIntersection(&mouseRect, replayFromStart.getHitbox()))
                {
                    return 1;
                }
                else if (SDL_HasIntersection(&mouseRect, replayLevel.getHitbox()))
                {
                    return 2;
                }
                else if (SDL_HasIntersection(&mouseRect, quitAndSave.getHitbox()))
                {
                    return 3;
                }
                else if (SDL_HasIntersection(&mouseRect, quit.getHitbox()))
                {
                    Quit(window);
                }
            }
        }

        window.clear();
        window.renderTexture1(backroundWin, {0, 0, 1920, 1080});
        window.renderTexture1(image, {746, 72, 427, 452});
        quit.render(window, mouseRect);
        quitAndSave.render(window, mouseRect);
        replayLevel.render(window, mouseRect);
        replayFromStart.render(window, mouseRect);
        window.display();
    }
    return 0;
}