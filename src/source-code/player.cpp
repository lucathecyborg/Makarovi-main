#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

#include "Render.hpp"
#include "Entity.hpp"
#include "Player.hpp"


Player::Player(){};
Player::Player(Entity *player1){
    player=player1;
}
void Player::setTex(SDL_Texture* p_tex){
    player->setTex(p_tex);
}

void Player::Damage(int damage){
    
    health-=damage;
    std::cout<<damage<<"   "<<health<<std::endl;
    if(health>100){
        health=100;
    }
    else if(health<0){
        health=0;
    }
    else{}
    
}

bool Player::Alive(){
    if(health<=0){
        return false;
    }
    if(health>0){
        return true;
    }
   return true;
}

float &Player::getHealth(){
    return health;
}

void Player::setHealth(int helt){
    health=helt;
}