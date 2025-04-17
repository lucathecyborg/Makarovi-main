#pragma once
#include "Entity.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>
#include <fstream>

void saveReplay(Level &level, vector<char> movement);

void playReplay(Level &level, vector<char> movement, Render &window);