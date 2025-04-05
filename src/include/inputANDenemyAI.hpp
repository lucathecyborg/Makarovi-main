#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Entity.hpp"
#include "Player.hpp"
#include "levels.hpp"
#include "Render.hpp"
#include <vector>

void enemyAI(std::vector<Entity>& enemies, int number, Level levels[], int level_counter, int modifier);

bool inputHandling(SDL_Event event, bool &gameRunning, Player &player1, SDL_Texture *player_Walking_Forward[], SDL_Texture *player_Walking_Backward[], bool &punch, Render &window, bool &moveUp, bool &moveDown, bool &moveLeft, bool &moveRight, Level level[], int &level_counter);

bool playerSetup(Entity &player, SDL_Texture *mapTex, Render window, SDL_Rect srcRect, SDL_Texture *player_walking_backwards[], Level &currentLevel, int modifyer);

void gameOver(Render window, TTF_Font *font);

void Quit(Render window);


void writeText(Render window);