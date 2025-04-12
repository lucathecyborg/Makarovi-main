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

int main(int argc, char *args[])
{
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

    Mix_Music *menuMusic = Mix_LoadMUS("src/res/sounds/menu.mp3");
    if (!menuMusic)
    {
        std::cerr << "Failed to load menu music: " << Mix_GetError() << std::endl;
    }
    else
    {
        Mix_PlayMusic(menuMusic, -1);
    }

    Render window("Johnny Englishhh", 1920, 1080);

    TTF_Font *font = TTF_OpenFont("src/res/dev/ROGFonts-Regular.otf", 24);
    SDL_Color textColor = {255, 0, 0, 200};
    SDL_Texture *player_Walking_Forward[4];
    SDL_Texture *player_Walking_Backward[4];

    player_Walking_Backward[0] = window.loadTexture("src/res/gfx/ppl_textures/player/moving backward/moving b1.png");
    player_Walking_Backward[1] = window.loadTexture("src/res/gfx/ppl_textures/player/moving backward/moving b2.png");
    player_Walking_Backward[2] = window.loadTexture("src/res/gfx/ppl_textures/player/moving backward/moving b3.png");
    player_Walking_Backward[3] = window.loadTexture("src/res/gfx/ppl_textures/player/moving backward/moving b4.png");

    SDL_Texture *clueDist[6];
    clueDist[0] = window.loadTexture("src/res/gfx/signal0.png");
    clueDist[1] = window.loadTexture("src/res/gfx/signal1.png");
    clueDist[2] = window.loadTexture("src/res/gfx/signal2.png");
    clueDist[3] = window.loadTexture("src/res/gfx/signal3.png");
    clueDist[4] = window.loadTexture("src/res/gfx/signal4.png");
    clueDist[5] = window.loadTexture("src/res/gfx/signal_full.png");

    SDL_Texture *playerTex = window.loadTexture("src/res/gfx/player.png");
    Entity player(960, 540, playerTex, true);

    SDL_Texture *map3Tex = window.loadTexture("src/res/dev/map3.png");
    SDL_Texture *map2Tex = window.loadTexture("src/res/dev/map2.png");
    SDL_Texture *map1Tex = window.loadTexture("src/res/dev/map1.png");

    SDL_Texture *startup1 = window.loadTexture("src/res/dev/startup1.png");
    SDL_Texture *startup2 = window.loadTexture("src/res/dev/startup2.png");
    SDL_Texture *startup3 = window.loadTexture("src/res/dev/startup1.png");

    SDL_Texture *gateClosed = window.loadTexture("src/res/dev/gate1.png");
    SDL_Texture *gateOpen = window.loadTexture("src/res/dev/gate1_open.png");

    SDL_Texture *jailTexO = window.loadTexture("src/res/dev/jailOpen.png");
    SDL_Texture *jailTexC = window.loadTexture("src/res/dev/jailClosed.png");

    Level levels[3] = {Level(1, 4668, 2626, 1, 5, window, map1Tex, startup1, gateClosed, gateOpen, Mix_LoadMUS("src/res/sounds/desert level.mp3")), Level(2, 6966, 3918, 2, 15, window, map2Tex, startup2, gateClosed, gateOpen, Mix_LoadMUS("src/res/sounds/jungle.mp3")), Level(3, 8120, 4567, 3, 20, window, map3Tex, startup1, jailTexC, jailTexO, Mix_LoadMUS("src/res/sounds/city.mp3"))};
    levels[0].loadPlayer(player);
    int level_counter = 0;
    bool gameRunning = true;

    level_counter = menu(window, levels, level_counter); // Use the updated menu function
    Mix_HaltMusic();
    std::cout << level_counter << std::endl;
    bool win = false;
    bool playersetup1;

    int lastPunchTime = 0;
    int movementSpeed = 2;
    int modifier;
    int redCooldown = 0;
    SDL_DisplayMode displayMode;
    int selection;
    if (SDL_GetCurrentDisplayMode(0, &displayMode) == 0)
    {
        std::cout << "Refresh Rate: " << displayMode.refresh_rate << " Hz" << std::endl;
    }
    else
    {
        std::cerr << "Could not get display mode! SDL_Error: " << SDL_GetError() << std::endl;
    }

    if (displayMode.refresh_rate > 100 && displayMode.refresh_rate < 146)
    {
        movementSpeed = 2;
        modifier = 1;
    }
    else if (displayMode.refresh_rate > 65 && displayMode.refresh_rate < 99)
    {
        movementSpeed = 3;
        modifier = 3;
    }
    else if (displayMode.refresh_rate > 29 && displayMode.refresh_rate < 64)
    {
        movementSpeed = 5;
        modifier = 4;
    }

    if (gameRunning)
        playersetup1 = playerSetup(player, levels[level_counter].getTex(), window, levels[level_counter].getSrcRect(), player_Walking_Backward, levels[level_counter], modifier);

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

    player_Walking_Forward[1] = window.loadTexture("src/res/gfx/ppl_textures/player/moving forward/moving f2.png");
    player_Walking_Forward[2] = window.loadTexture("src/res/gfx/ppl_textures/player/moving forward/moving f3.png");
    player_Walking_Forward[3] = window.loadTexture("src/res/gfx/ppl_textures/player/moving forward/moving f4.png");

    SDL_Event event;
    SDL_Rect tempRect;

    SDL_Texture *enemyTex = window.loadTexture("src/res/gfx/idle.png");
    SDL_Texture *healthPackTex = window.loadTexture("src/res/gfx/healthpack.png");
    SDL_Texture *exclamationTex = window.loadTexture("src/res/gfx/!.png");
    SDL_Rect playerRect = {960, 540, 100, 100};

    Entity healthPack(500, 500, healthPackTex, true);

    Text text1(window.getRenderer(), textColor, font, "Health: " + std::to_string(static_cast<int>(levels[level_counter].getPlayer().getHealth())), textWidth, 0, 150, 400);

    // Initialize enemies array
    srand(time(NULL));

    struct LastMove
    {
        bool Up = false;
        bool Down = false;
        bool Left = false;
        bool Right = false;
    } lastMove;

    // GAME LOOP
    while (gameRunning)
    {
        ticks = SDL_GetTicks();
        bool moveUp = false, moveDown = false, moveLeft = false, moveRight = false;
        int tempCount = level_counter;
        bool input = inputHandling(event, gameRunning, levels[level_counter].getPlayer(), player_Walking_Forward, player_Walking_Backward, punch, window, moveUp, moveDown, moveLeft, moveRight, levels, level_counter);

        if (tempCount != level_counter)
            goto setup;

        tempRect = playerRect;

        // MOVEMENT ------------------------------------------------------------------------------------------------------------------------------------------------
        tempRect.y -= 10;
        if (!levels[level_counter].checkCollision(&tempRect))
        {
            if (moveUp && levels[level_counter].getSrcRect().y - movementSpeed >= 0)
            {
                healthPack.Move(healthPack.getX(), healthPack.getY() + movementSpeed);

                levels[level_counter].moveAll(0, movementSpeed);

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
            if (moveDown && levels[level_counter].getSrcRect().y + movementSpeed <= levels[level_counter].getY() - 1180)
            {

                healthPack.Move(healthPack.getX(), healthPack.getY() - movementSpeed);

                levels[level_counter].moveAll(0, -movementSpeed);
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
            if (moveLeft && levels[level_counter].getSrcRect().x - movementSpeed >= 0)
            {

                healthPack.Move(healthPack.getX() + movementSpeed, healthPack.getY());

                levels[level_counter].moveAll(movementSpeed, 0);
                lastMove.Left = true;
            }
        }
        tempRect = playerRect;
        tempRect.x += 10;
        if (!levels[level_counter].checkCollision(&tempRect))
        {
            if (moveRight && levels[level_counter].getSrcRect().x + movementSpeed <= levels[level_counter].getX() - 2020)
            {

                healthPack.Move(healthPack.getX() - movementSpeed, healthPack.getY());

                levels[level_counter].moveAll(-movementSpeed, 0);
                lastMove.Right = true;
            }
        }
        // END OF MOVEMENT ------------------------------------------------------------------------------------------------------------------------------------------------

        enemyAI(levels[level_counter].getEnemies(), levels[level_counter].getEnemyNumber(), levels, level_counter, modifier);

        if (SDL_HasIntersection(&playerRect, healthPack.getHitbox()) && levels[level_counter].getPlayer().getHealth() != 100 && healthPack.Alive() == true)
        {
            levels[level_counter].getPlayer().Damage(-50);
            healthPack.setAlive(false);
            heal = true;
            healtime = ticks;
        }

        // DAMAGE
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
                }
            }

            // PUNCHING
            if ((levels[level_counter].useEnemy(i).Circle(levels[level_counter].useEnemy(i).getX(), levels[level_counter].useEnemy(i).getY(), 200) && levels[level_counter].useEnemy(i).Alive() == true))
            {
                renderEx = true;
                SDL_Rect exclamationRect = {playerRect.x + (playerRect.w / 2) - 50, playerRect.y - 100, 100, 100};
                if (punch == true && ticks - lastPunchTime > 400)
                {
                    levels[level_counter].useEnemy(i).setAlive(false);
                    punch = false;
                    lastPunchTime = ticks;
                }
            }
        }
        punch = false;

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

        if (levels[level_counter].getPlayer().Alive() == false || !levels[level_counter].checkDeathCollision(player.getHitbox()))
        {
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
        window.renderEntity(healthPack, healthPack.getX(), healthPack.getY(), 50, 50, healthPack.Alive());
        window.renderPlayer(&player);
        text1.setText("Health: " + std::to_string(static_cast<int>(levels[level_counter].getPlayer().getHealth())));
        text1.renderText1(textWidth, textHeight);
        if (renderEx == true)
        {
            window.renderTexture1(exclamationTex, {960, 440, 100, 100});
            renderEx = false;
        }

        window.renderTexture1(clueDist[levels[level_counter].clueDistance()], {1644, 819, 255, 240});

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
        if (levels[level_counter].checkComplete())
        {
            if (SDL_HasIntersection(&levels[level_counter].getgateRect(), &playerRect))
            {
                if (level_counter == 2)
                {
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
                        playersetup1 = playerSetup(player, levels[level_counter].getTex(), window, levels[level_counter].getSrcRect(), player_Walking_Backward, levels[level_counter], modifier);
                        player.setTex(player_Walking_Forward[0]);
                        player.Move(960, 540);
                    }
                }
            }
        }
    }

Finish:

    Quit(window);

    return 0;
}
