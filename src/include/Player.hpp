#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Render.hpp"
#include "Entity.hpp"

class Player
{
private:
    float health = 100;
    Entity *player;

public:
    Player(Entity *player1);
    void setTex(SDL_Texture *p_tex);
    void Damage(int damage);
    bool Alive();
    float &getHealth();
    Entity *getPlayer1() { return player; }
    Player();
    void setHealth(int helt);
    void setAlive(bool alive);
};
