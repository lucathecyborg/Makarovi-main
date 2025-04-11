
#include <SDL2/SDL.h>
#include <vector>
#include "levels.hpp"
int menu(Render &window, Level levels[], int &levelNumber);
void credits(Render &window);
void options(Render &window);
void tutorial(Render &window);
int selectScreen(Render &window, bool win);