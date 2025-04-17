
#include <SDL2/SDL.h>
#include <vector>
#include "levels.hpp"
int menu(Render &window, Level levels[], int &levelNumber, vector<char> movement);
void credits(Render &window);
void options(Render &window);
void tutorial(Render &window);
int selectScreen(Render &window, bool win);
void Replay(Render &window, Level levels[], vector<char> movement);