#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <ctime>
#include <string>
#include <vector>
#include <fstream>

#include "Render.hpp"
#include "entity.hpp"
#include "Player.hpp"
#include "button.hpp"

class Clue;

class Level
{
    int levelNumber;
    int sizeX;
    int sizeY;
    SDL_Texture *mapTex;
    SDL_Rect srcRect;
    SDL_Rect dstRect = {0, 0, 1920, 1080};
    int enemyType;
    int enemyNumber;
    Render window;
    vector<Entity> enemies;
    vector<SDL_Rect> walls;
    vector<SDL_Rect> deathBarriers;
    vector<Clue> clues;
    vector<Entity> healthPacks;
    bool levelComplete = false;
    SDL_Texture *setupTex;
    SDL_Texture *gateOpen;
    SDL_Texture *gateClosed;
    SDL_Rect gateRectOpen;
    SDL_Rect gateRectClosed;
    Entity *scientist;
    SDL_Texture *clueRoomTex;

public:
    static Player player1;
    Level(int levelNumber, int sizeX, int sizeY, int enemyType, int enemyNumber, Render window, SDL_Texture *Tex, SDL_Texture *setTex, SDL_Texture *gateC, SDL_Texture *gateO);
    void render();
    SDL_Texture *getTex() { return mapTex; }
    SDL_Rect getSrcRect() { return srcRect; }
    SDL_Rect getDstRect() { return dstRect; }
    int getX() { return sizeX; }
    int getY() { return sizeY; }
    void createEnemies();
    Entity &useEnemy(int i);
    vector<Entity> &getEnemies();
    int getEnemyNumber();
    void resetLevel();
    vector<SDL_Rect> &getWalls();
    void moveAll(int x, int y);
    bool checkCollision(SDL_Rect *playerHitbox);
    void createWalls();
    bool enemyCheckCollision(SDL_Rect *enemyHitbox);
    bool checkComplete();
    bool checkDeathCollision(SDL_Rect *playerHitbox);
    void createClues();
    bool checkClues(Entity *player1);
    vector<Clue> &getClues();
    Render getWindow() { return window; };
    void start();
    int clueDistance();
    SDL_Rect &getgateRect();
    void loadPlayer(Entity &x);
    Player &getPlayer() { return player1; };
    void saveToFile(const std::string &filename);
    void loadFromFile(const std::string &filename, Render &window);
    int numberCheck();
    void renderCounter(Render &window);
    void clueRoom(Render &window);
};
