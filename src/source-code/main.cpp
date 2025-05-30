#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <ctime>
#include <string>
#include <SDL2/SDL_mixer.h>

#include "Render.hpp"
#include "Entity.hpp"
#include "Player.hpp"
#include "menu.hpp"
#include "inputANDenemyAI.hpp"
#include "levels.hpp"
#include "replay.hpp"

int main(int argc, char *args[])
{
    // Inicializacija knjižnjic
    if (SDL_Init(SDL_INIT_EVERYTHING) > 0)
    {
        std::cout << "HEY.. SDL_Init HAS FAILED. SDL_ERROR: " << SDL_GetError() << std::endl;
        return -1;
    }

    if (!(IMG_Init(IMG_INIT_PNG)))
    {
        std::cout << "IMG_init has failed. Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    if ((TTF_Init() == -1))
    {
        std::cout << "TTF_Init has failed. Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        std::cerr << "SDL_mixer Init error: " << Mix_GetError() << "\n";
        return -1;
    }

    // Sounds
    Mix_Music *menuMusic = Mix_LoadMUS("src/res/sounds/menu.mp3");
    if (!menuMusic)
    {
        std::cerr << "Failed to load menu music: " << Mix_GetError() << std::endl;
    }
    else
    {
        Mix_FadeInMusic(menuMusic, -1, 5000);
    }

    Mix_Chunk *damageSound = Mix_LoadWAV("src/res/sounds/damage.wav");
    Mix_Chunk *punchSound = Mix_LoadWAV("src/res/sounds/punch.wav");

    // Ustvarimo okno, loading screen ter ustvarimo potrebne spremenljivke
    Render window("Johnny Englishhh", 1920, 1080);
    window.renderTexture1(window.loadTexture("src/res/dev/loading.png"), {0, 0, 1920, 1080});
    window.display();
    TTF_Font *font = TTF_OpenFont("src/res/dev/ROGFonts-Regular.otf", 60);
    SDL_Color textColor = {255, 0, 0, 200};
    SDL_Texture *player_Walking_Forward[4];
    SDL_Texture *player_Walking_Backward[4];

    player_Walking_Backward[0] = window.loadTexture("src/res/gfx/ppl_textures/player/moving backward/moving b1.png");
    player_Walking_Backward[1] = window.loadTexture("src/res/gfx/ppl_textures/player/moving backward/moving b2.png");
    player_Walking_Backward[2] = window.loadTexture("src/res/gfx/ppl_textures/player/moving backward/moving b3.png");
    player_Walking_Backward[3] = window.loadTexture("src/res/gfx/ppl_textures/player/moving backward/moving b4.png");

    player_Walking_Forward[0] = window.loadTexture("src/res/gfx/ppl_textures/player/moving forward/moving f1.png");
    player_Walking_Forward[1] = window.loadTexture("src/res/gfx/ppl_textures/player/moving forward/moving f2.png");
    player_Walking_Forward[2] = window.loadTexture("src/res/gfx/ppl_textures/player/moving forward/moving f3.png");
    player_Walking_Forward[3] = window.loadTexture("src/res/gfx/ppl_textures/player/moving forward/moving f4.png");

    Entity player(960, 540, player_Walking_Forward[4], true);

    SDL_Texture *map3Tex = window.loadTexture("src/res/dev/map3.png");
    SDL_Texture *map2Tex = window.loadTexture("src/res/dev/map2.png");
    SDL_Texture *map1Tex = window.loadTexture("src/res/dev/map1.png");

    SDL_Texture *startup1 = window.loadTexture("src/res/dev/startup1.png");
    SDL_Texture *startup2 = window.loadTexture("src/res/dev/startup2.png");
    SDL_Texture *startup3 = window.loadTexture("src/res/dev/startup3.png");

    SDL_Texture *gateClosed = window.loadTexture("src/res/dev/gate1.png");
    SDL_Texture *gateOpen = window.loadTexture("src/res/dev/gate1_open.png");

    SDL_Texture *jailTexO = window.loadTexture("src/res/dev/jailOpen.png");
    SDL_Texture *jailTexC = window.loadTexture("src/res/dev/jailClosed.png");

    // Ustvarimo levele, igralca, izberemo hitrost, ter kličemo prvi meni
    Level levels[3] = {Level(1, 4668, 2626, 1, 5, window, map1Tex, startup1, gateClosed, gateOpen, Mix_LoadMUS("src/res/sounds/desert level.mp3")), Level(2, 6966, 3918, 2, 15, window, map2Tex, startup2, gateClosed, gateOpen, Mix_LoadMUS("src/res/sounds/jungle.mp3")), Level(3, 8120, 4567, 3, 20, window, map3Tex, startup3, jailTexC, jailTexO, Mix_LoadMUS("src/res/sounds/city.mp3"))};
    levels[0].loadPlayer(player);
    levels[0].setSpeed();
    int level_counter = 0;
    bool gameRunning = true;
    window.clear();

    // Pripravimo za igro
    vector<char> playerMovement; // bo vsebovala vse premike
    level_counter = menu(window, levels, level_counter, playerMovement);
    Mix_HaltMusic();
    std::cout << level_counter << std::endl;
    bool win = false;
    bool playersetup1;

    SDL_Texture *clueDist[6];
    clueDist[0] = window.loadTexture("src/res/gfx/signal0.png");
    clueDist[1] = window.loadTexture("src/res/gfx/signal1.png");
    clueDist[2] = window.loadTexture("src/res/gfx/signal2.png");
    clueDist[3] = window.loadTexture("src/res/gfx/signal3.png");
    clueDist[4] = window.loadTexture("src/res/gfx/signal4.png");
    clueDist[5] = window.loadTexture("src/res/gfx/signal_full.png");
    int lastPunchTime = 0;

    int redCooldown = 0;
    int selection;

    // Setup animacija
    if (gameRunning)
        playersetup1 = playerSetup(player, levels[level_counter].getTex(), window, levels[level_counter].getSrcRect(), player_Walking_Backward, levels[level_counter], levels[level_counter].getModifier());

    player_Walking_Forward[0] = window.loadTexture("src/res/gfx/ppl_textures/player/moving forward/moving f1.png");
    player.Move(960, 540);
    player.setTex(player_Walking_Forward[0]);

    int textWidth = 1900;
    int textHeight = 120;
    bool checking = true;
    int lastcheck;
    bool heal = false;
    int healtime = 0;

    int animation_stage = 0;
    int ticks;
    int Current_ticks = 0;
    uint32_t lastDamageTime = 0;
    int animation_time = 100;
    bool punch = false;
    bool renderEx = false;
    bool damageTime = false;
    SDL_Rect damageRect = {0, 0, 1920, 1080};

    SDL_Event event;
    SDL_Rect tempRect;

    SDL_Texture *exclamationTex = window.loadTexture("src/res/gfx/!.png");
    SDL_Rect playerRect = {960, 540, 100, 100};

    // Ustvarimo health text
    Text text1(window.getRenderer(), textColor, font, "Health: " + std::to_string(static_cast<int>(levels[level_counter].getPlayer().getHealth())), textWidth, 0, 150, 400);

    srand(time(NULL));

    // Strutc za pozicijo nasprotnikov
    struct LastMove
    {
        bool Up = false;
        bool Down = false;
        bool Left = false;
        bool Right = false;
    } lastMove;

    int frameCount = 0;
    uint32_t fpsTimer = SDL_GetTicks();

    // GAME LOOP
    while (gameRunning)
    {
        // FPS counter
        uint32_t currentFrameTime = SDL_GetTicks();
        frameCount++;
        bool moved = false;
        if (currentFrameTime - fpsTimer >= 1000)
        {
            int fps = frameCount;
            std::cout << "FPS: " << fps << std::endl;
            frameCount = 0;
            fpsTimer = currentFrameTime;
        }

        ticks = SDL_GetTicks();
        bool moveUp = false, moveDown = false, moveLeft = false, moveRight = false;
        int tempCount = level_counter;
        // Kličemo vse inputs
        bool input = inputHandling(event, gameRunning, levels[level_counter].getPlayer(), player_Walking_Forward, player_Walking_Backward, punch, window, moveUp, moveDown, moveLeft, moveRight, levels, level_counter, playerMovement);

        // Preverimo če smo spremenili level s nalaganjem
        if (tempCount != level_counter)
            goto setup;

        tempRect = playerRect;

        // MOVEMENT ------------------------------------------------------------------------------------------------------------------------------------------------
        tempRect.y -= 10;
        if (!levels[level_counter].checkCollision(&tempRect))
        {
            if (moveUp && levels[level_counter].getSrcRect().y - levels[level_counter].getSpeed() >= 0)
            {
                playerMovement.push_back('u');
                moved = true;
                levels[level_counter].moveAll(0, levels[level_counter].getSpeed());

                lastMove.Up = true;
                if (ticks - Current_ticks >= animation_time)
                {
                    levels[level_counter].getPlayer().setTex(player_Walking_Backward[animation_stage]);
                    animation_stage = (animation_stage + 1) % 4;
                    Current_ticks = ticks;
                }
            }
        }
        tempRect = playerRect;
        tempRect.y += 10;
        if (!levels[level_counter].checkCollision(&tempRect))
        {

            if (moveDown && levels[level_counter].getSrcRect().y + levels[level_counter].getSpeed() <= levels[level_counter].getY() - 1180)
            {
                playerMovement.push_back('d');
                moved = true;
                levels[level_counter].moveAll(0, -levels[level_counter].getSpeed());
                lastMove.Down = true;
                if (ticks - Current_ticks >= animation_time)
                {
                    levels[level_counter].getPlayer().setTex(player_Walking_Forward[animation_stage]);
                    animation_stage = (animation_stage + 1) % 4;
                    Current_ticks = ticks;
                }
            }
        }
        tempRect = playerRect;
        tempRect.x -= 10;
        if (!levels[level_counter].checkCollision(&tempRect))
        {

            if (moveLeft && levels[level_counter].getSrcRect().x - levels[level_counter].getSpeed() >= 0)
            {
                playerMovement.push_back('l');
                moved = true;
                levels[level_counter].moveAll(levels[level_counter].getSpeed(), 0);
                lastMove.Left = true;
            }
        }
        tempRect = playerRect;
        tempRect.x += 10;
        if (!levels[level_counter].checkCollision(&tempRect))
        {
            if (moveRight && levels[level_counter].getSrcRect().x + levels[level_counter].getSpeed() <= levels[level_counter].getX() - 2020)
            {
                playerMovement.push_back('r');
                moved = true;
                levels[level_counter].moveAll(-levels[level_counter].getSpeed(), 0);
                lastMove.Right = true;
            }
        }
        // END OF MOVEMENT ------------------------------------------------------------------------------------------------------------------------------------------------

        // Če ni bilo premika shranimo ' ', da se bo zaznalo na replay
        if (!moved)
            playerMovement.push_back(' ');

        // Kličemo enemyAI
        enemyAI(levels[level_counter].getEnemies(), levels[level_counter].getEnemyNumber(), levels, level_counter, levels[level_counter].getModifier());

        // Preverimo če smo zadeli healthPack
        if (ticks - healtime > 200)
        {
            heal = levels[level_counter].checkHeal(&playerRect);
            if (heal)
            {
                healtime = ticks;
            }
        }

        // Preverimo če smo dobili damage od nasprotnika
        for (int i = 0; i < levels[level_counter].getEnemyNumber(); i++)
        {
            if (SDL_HasIntersection(&playerRect, levels[level_counter].useEnemy(i).getHitbox()))
            {
                if (levels[level_counter].useEnemy(i).Alive() == true && ticks - lastDamageTime > 1000)
                {
                    levels[level_counter].getPlayer().Damage(10);

                    lastDamageTime = ticks;
                    damageTime = true;
                    redCooldown = ticks;
                    Mix_PlayChannel(-1, damageSound, 0);
                }
            }

            // Preverimo če lahko/smo udarili nasprotnika
            if ((levels[level_counter].useEnemy(i).Circle(levels[level_counter].useEnemy(i).getX(), levels[level_counter].useEnemy(i).getY(), 200) && levels[level_counter].useEnemy(i).Alive() == true))
            {
                // Če je nasprotnik dovolj blizu, prikažemo klicaj nad glavo
                renderEx = true;
                SDL_Rect exclamationRect = {playerRect.x + (playerRect.w / 2) - 50, playerRect.y - 100, 100, 100};
                if (punch == true && ticks - lastPunchTime > 400)
                {
                    Mix_PlayChannel(-1, punchSound, 0);
                    levels[level_counter].useEnemy(i).setAlive(false);
                    punch = false;
                    lastPunchTime = ticks;
                }
            }
        }
        punch = false;

        // Preverimo če smo trčili s uganko
        if (checking)
        {
            checking = levels[level_counter].checkClues(&player);
            lastcheck = ticks;
        }
        if (!checking)
        {
            if (ticks - lastcheck > 3000)
                checking = true;
        }

        // Preverimo če smo umrli
        if (levels[level_counter].getPlayer().Alive() == false || !levels[level_counter].checkDeathCollision(player.getHitbox()))
        {
            playerMovement.push_back('x');
            saveReplay(levels[level_counter], playerMovement);
            playerMovement.clear();
            gameOver(window, font, win);
            selection = selectScreen(window, win);
            switch (selection)
            {
            case 1:
                for (int i = 0; i < 3; i++)
                {
                    levels[i].resetLevel();
                }
                level_counter = 0;
                goto setup;
                break;
            case 2:
                levels[level_counter].resetLevel();
                goto setup;
                break;
            case 3:
                levels[level_counter].resetLevel();
                levels[level_counter].saveToFile();
                gameRunning = false;
                break;
            }
        }

        // RENDERING
        window.clear();

        levels[level_counter].render();
        levels[level_counter].renderCounter(window);
        text1.setText("Health: " + std::to_string(static_cast<int>(levels[level_counter].getPlayer().getHealth())));
        text1.renderText1(textWidth, textHeight);
        if (renderEx == true)
        {
            window.renderTexture1(exclamationTex, {960, 440, 100, 100});
            renderEx = false;
        }

        window.renderTexture1(clueDist[levels[level_counter].clueDistance()], {1644, 819, 255, 240});

        // Prikaz rdečega ob udarcu
        if (damageTime == true)
        {

            SDL_SetRenderDrawBlendMode(window.getRenderer(), SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(window.getRenderer(), 255, 0, 0, 60);
            SDL_RenderFillRect(window.getRenderer(), &damageRect);
            SDL_SetRenderDrawBlendMode(window.getRenderer(), SDL_BLENDMODE_NONE);
            if (ticks - redCooldown > 200)
            {
                damageTime = false;
            }
        }

        // Prikaz zelenega ob healanju
        if (heal == true)
        {

            SDL_SetRenderDrawBlendMode(window.getRenderer(), SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(window.getRenderer(), 0, 255, 0, 60);
            SDL_RenderFillRect(window.getRenderer(), &damageRect);
            SDL_SetRenderDrawBlendMode(window.getRenderer(), SDL_BLENDMODE_NONE);
            if (ticks - healtime > 200)
            {
                heal = false;
            }
        }

        window.display();

        // Preverimo če smo uspešno končali level
        // Če smo, gremo na nasljednjega, če smo pa zmagali celo igro pokažemo "Win menu"
        if (levels[level_counter].checkComplete())
        {
            if (SDL_HasIntersection(&levels[level_counter].getgateRect(), &playerRect))
            {
                playerMovement.push_back('n');
                saveReplay(levels[level_counter], playerMovement);
                playerMovement.clear();
                if (level_counter == 2)
                {
                    playerMovement.push_back('w');
                    playerMovement.clear();
                    win = true;
                    gameOver(window, font, win);
                    selection = selectScreen(window, win);
                    switch (selection)
                    {
                    case 1:
                        for (int i = 0; i < 3; i++)
                        {
                            levels[i].resetLevel();
                        }
                        level_counter = 0;
                        goto setup;
                        break;
                    case 2:
                        levels[level_counter].resetLevel();
                        goto setup;
                        break;
                    case 3:
                        levels[level_counter].resetLevel();
                        levels[level_counter].saveToFile();
                        gameRunning = false;
                        break;
                    }
                }
                else
                {
                    levels[level_counter].stop();
                    level_counter++;
                    if (level_counter == 3)
                    {
                        gameRunning = false;
                    }
                    else
                    {
                    setup:
                        std::cout << "setup: " << level_counter << std::endl;
                        playersetup1 = playerSetup(player, levels[level_counter].getTex(), window, levels[level_counter].getSrcRect(), player_Walking_Backward, levels[level_counter], levels[level_counter].getModifier());
                        player.setTex(player_Walking_Forward[0]);
                        player.Move(960, 540);
                    }
                }
            }
        }
    }

// CleanUp
Finish:
    SDL_DestroyTexture(player_Walking_Forward[0]);
    SDL_DestroyTexture(player_Walking_Forward[1]);
    SDL_DestroyTexture(player_Walking_Forward[2]);
    SDL_DestroyTexture(player_Walking_Forward[3]);
    SDL_DestroyTexture(player_Walking_Backward[0]);
    SDL_DestroyTexture(player_Walking_Backward[1]);
    SDL_DestroyTexture(player_Walking_Backward[2]);
    SDL_DestroyTexture(player_Walking_Backward[3]);
    SDL_DestroyTexture(map1Tex);
    SDL_DestroyTexture(map2Tex);
    SDL_DestroyTexture(map3Tex);
    SDL_DestroyTexture(startup1);
    SDL_DestroyTexture(startup2);
    SDL_DestroyTexture(startup3);
    SDL_DestroyTexture(gateClosed);
    SDL_DestroyTexture(gateOpen);
    SDL_DestroyTexture(jailTexO);
    SDL_DestroyTexture(jailTexC);
    SDL_DestroyTexture(exclamationTex);
    SDL_DestroyTexture(clueDist[0]);
    SDL_DestroyTexture(clueDist[1]);
    SDL_DestroyTexture(clueDist[2]);
    SDL_DestroyTexture(clueDist[3]);
    SDL_DestroyTexture(clueDist[4]);
    SDL_DestroyTexture(clueDist[5]);

    Quit(window);

    return 0;
}
