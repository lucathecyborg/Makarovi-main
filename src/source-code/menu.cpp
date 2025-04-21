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
#include "replay.hpp"

// Main menu funkcija
int menu(Render &window, Level levels[], int &levelNumber, vector<char> movement)
{

    bool menu = true;
    SDL_Event event;
    SDL_Texture *menuTex = window.loadTexture("src/res/dev/main menu.png");

    Button start(771, 380, 378, 86, window.loadTexture("src/res/dev/play.png"), window.loadTexture("src/res/dev/play pressed.png"));
    Button quit(771, 850, 378, 86, window.loadTexture("src/res/dev/quit.png"), window.loadTexture("src/res/dev/quit pressed.png"));
    Button creditsButton(771, 692, 378, 86, window.loadTexture("src/res/dev/credits.png"), window.loadTexture("src/res/dev/credits pressed.png"));
    Button optionsButton(771, 540, 378, 86, window.loadTexture("src/res/dev/options.png"), window.loadTexture("src/res/dev/options pressed.png"));
    Button saveButon(135, 423, 378, 86, window.loadTexture("src/res/dev/save.png"), window.loadTexture("src/res/dev/save pressed.png"));
    Button loadButon(135, 546, 378, 86, window.loadTexture("src/res/dev/load.png"), window.loadTexture("src/res/dev/load pressed.png"));
    Button tutbutton(1407, 772, 378, 86, window.loadTexture("src/res/dev/tutorial/Tutorial.png"), window.loadTexture("src/res/dev/tutorial/tutorial_pressed.png"));
    Button clueButton(135, 772, 378, 86, window.loadTexture("src/res/dev/clue.png"), window.loadTexture("src/res/dev/clue pressed.png"));
    Button replayV(1407, 546, 422, 86, window.loadTexture("src/res/dev/replayV.png"), window.loadTexture("src/res/dev/replayV pressed.png"));
    Button replayS(1407, 423, 380, 86, window.loadTexture("src/res/dev/replayS.png"), window.loadTexture("src/res/dev/replayS pressed.png"));
    while (menu)
    {

        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        SDL_Rect mouseRect = {mouseX, mouseY, 1, 1};
        window.renderTexture(menuTex, {0, 0, 1920, 1080}, {0, 0, 1920, 1080});
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
                        TTF_CloseFont(Savefont);
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
                        TTF_CloseFont(Loadfont);
                    }
                    else if (SDL_HasIntersection(&mouseRect, clueButton.getHitbox()))
                    {
                        levels[levelNumber].clueRoom(window);
                    }
                    else if (SDL_HasIntersection(&mouseRect, replayV.getHitbox()))
                    {
                        Replay(window, levels, movement);
                    }
                    else if (SDL_HasIntersection(&mouseRect, replayS.getHitbox()))
                    {
                        saveReplay(levels[levelNumber], movement);
                        TTF_Font *Savefont = TTF_OpenFont("src/res/dev/IBMPlexMono-Medium.ttf", 140);
                        Text saveText(window.getRenderer(), {255, 0, 0, 255}, Savefont, "Replay Saved", 500, 500, 400, 1000, {0, 0, 0, 255});
                        saveText.renderText(450, 300);
                        window.display();
                        SDL_Delay(2000);
                        TTF_CloseFont(Savefont);
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

        start.render(window, mouseRect);
        quit.render(window, mouseRect);
        creditsButton.render(window, mouseRect);
        optionsButton.render(window, mouseRect);
        saveButon.render(window, mouseRect);
        loadButon.render(window, mouseRect);
        clueButton.render(window, mouseRect);
        tutbutton.render(window, mouseRect);
        replayS.render(window, mouseRect);
        replayV.render(window, mouseRect);
        window.display();
        window.clear();
    }
    SDL_DestroyTexture(menuTex);
    SDL_DestroyTexture(start.getTex1());
    SDL_DestroyTexture(start.getTex2());
    SDL_DestroyTexture(quit.getTex1());
    SDL_DestroyTexture(quit.getTex2());
    SDL_DestroyTexture(creditsButton.getTex1());
    SDL_DestroyTexture(creditsButton.getTex2());
    SDL_DestroyTexture(optionsButton.getTex1());
    SDL_DestroyTexture(optionsButton.getTex2());
    SDL_DestroyTexture(saveButon.getTex1());
    SDL_DestroyTexture(saveButon.getTex2());
    SDL_DestroyTexture(loadButon.getTex1());
    SDL_DestroyTexture(loadButon.getTex2());
    SDL_DestroyTexture(clueButton.getTex1());
    SDL_DestroyTexture(clueButton.getTex2());
    SDL_DestroyTexture(tutbutton.getTex1());
    SDL_DestroyTexture(tutbutton.getTex2());
    SDL_DestroyTexture(replayS.getTex1());
    SDL_DestroyTexture(replayS.getTex2());
    SDL_DestroyTexture(replayV.getTex1());
    SDL_DestroyTexture(replayV.getTex2());

    return levelNumber;
}

// Credits menu
void credits(Render &window)
{

    bool credits = true;
    SDL_Event event;
    Button back(1507, 960, 378, 86, window.loadTexture("src/res/dev/quit3.png"), window.loadTexture("src/res/dev/quit pressed3.png"));
    SDL_Texture *creditsTex = window.loadTexture("src/res/dev/creditsScreen.png");
    while (credits)
    {
        window.renderTexture(creditsTex, {0, 0, 1920, 1080}, {0, 0, 1920, 1080});
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        SDL_Rect mouseRect = {mouseX, mouseY, 1, 1};
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
        back.render(window, mouseRect);
        window.display();
        window.clear();
    }

    SDL_DestroyTexture(creditsTex);
    SDL_DestroyTexture(back.getTex1());
    SDL_DestroyTexture(back.getTex2());
}

// options menu
void options(Render &window)
{
    bool options = true;
    SDL_Event event;
    float brightness = 0;
    Button back(1506, 979, 378, 86, window.loadTexture("src/res/dev/quit2.png"), window.loadTexture("src/res/dev/quit pressed2.png"));
    SDL_Texture *optionsTex = window.loadTexture("src/res/dev/optionsScreen.png");

    while (options)
    {
        window.renderTexture(optionsTex, {0, 0, 1920, 1080}, {0, 0, 1920, 1080});
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        SDL_Rect mouseRect = {mouseX, mouseY, 1, 1};
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
        back.render(window, mouseRect);
        window.display();
        window.clear();
    }

    SDL_DestroyTexture(optionsTex);
    SDL_DestroyTexture(back.getTex1());
    SDL_DestroyTexture(back.getTex2());
}

// Tutorial page
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
    SDL_DestroyTexture(pages[0]);
    SDL_DestroyTexture(pages[1]);
    SDL_DestroyTexture(pages[2]);
    SDL_DestroyTexture(pages[3]);
    SDL_DestroyTexture(pages[4]);
    SDL_DestroyTexture(pages[5]);
    SDL_DestroyTexture(arrowB);
    SDL_DestroyTexture(arrowF);
    SDL_DestroyTexture(arrowFP);
    SDL_DestroyTexture(arrowBP);
    SDL_DestroyTexture(backButton.getTex1());
    SDL_DestroyTexture(backButton.getTex2());
    SDL_DestroyTexture(frontButton.getTex1());
    SDL_DestroyTexture(frontButton.getTex2());
    SDL_DestroyTexture(back.getTex1());
    SDL_DestroyTexture(back.getTex2());
    SDL_DestroyTexture(buttonTex);
}

// Prikaže se ob smrti/zmagi, omogoča ponovno igranje, shranjevanje, zapuščanje igre...
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
    SDL_DestroyTexture(backroundWin);
    SDL_DestroyTexture(image);
    SDL_DestroyTexture(quit.getTex1());
    SDL_DestroyTexture(quit.getTex2());
    SDL_DestroyTexture(quitAndSave.getTex1());
    SDL_DestroyTexture(quitAndSave.getTex2());
    SDL_DestroyTexture(replayLevel.getTex1());
    SDL_DestroyTexture(replayLevel.getTex2());
    SDL_DestroyTexture(replayFromStart.getTex1());
    SDL_DestroyTexture(replayFromStart.getTex2());

    return 0;
}
// Prikaz menija za replay funkcijo (ni dejanski replay, to je v svoji datoteki!!)
void Replay(Render &window, Level levels[], vector<char> movement)
{
    Button replay1(158, 742, 372, 139, window.loadTexture("src/res/dev/replay.png"), window.loadTexture("src/res/dev/replayPressed.png"));
    Button replay2(774, 742, 372, 139, window.loadTexture("src/res/dev/replay.png"), window.loadTexture("src/res/dev/replayPressed.png"));
    Button replay3(1380, 742, 372, 139, window.loadTexture("src/res/dev/replay.png"), window.loadTexture("src/res/dev/replayPressed.png"));
    Button quit(1503, 958, 378, 92, window.loadTexture("src/res/dev/tutorial/quit.png"), window.loadTexture("src/res/dev/quit pressed2.png"));
    bool running = true;
    SDL_Event event;
    SDL_Texture *replayTex = window.loadTexture("src/res/dev/replayScreen.png");

    while (running)
    {
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        SDL_Rect mouseRect = {mouseX, mouseY, 1, 1};
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
            {
                if (SDL_HasIntersection(&mouseRect, replay1.getHitbox()))
                {
                    playReplay(levels[0], movement, window);
                }
                else if (SDL_HasIntersection(&mouseRect, replay2.getHitbox()))
                {
                    playReplay(levels[1], movement, window);
                }
                else if (SDL_HasIntersection(&mouseRect, replay3.getHitbox()))
                {
                    playReplay(levels[2], movement, window);
                }
                else if (SDL_HasIntersection(&mouseRect, quit.getHitbox()))
                {
                    running = false;
                }
            }
            if ((event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE))
            {
                running = false;
            }
        }

        window.clear();
        window.renderTexture1(replayTex, {0, 0, 1920, 1080});

        quit.render(window, mouseRect);
        replay1.render(window, mouseRect);
        replay2.render(window, mouseRect);
        replay3.render(window, mouseRect);
        window.display();
    }
    SDL_DestroyTexture(replayTex);
    SDL_DestroyTexture(replay1.getTex1());
    SDL_DestroyTexture(replay1.getTex2());
    SDL_DestroyTexture(replay2.getTex1());
    SDL_DestroyTexture(replay2.getTex2());
    SDL_DestroyTexture(replay3.getTex1());
    SDL_DestroyTexture(replay3.getTex2());
    SDL_DestroyTexture(quit.getTex1());
    SDL_DestroyTexture(quit.getTex2());
}