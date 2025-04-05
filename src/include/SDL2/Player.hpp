#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Render.hpp"
#include "Entity.hpp"


class Player{
    private:
        float health=100;
        Entity *player;
    public:
        Player(Entity *player1);
        void moveUP();
        void moveDOWN();
        void moveLEFT();
        void moveRIGHT();
        void Punch();
};
