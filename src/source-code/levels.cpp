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
#include "levels.hpp"









Player Level::player1 = Player();


Level::Level(int levelNumber, int sizeX, int sizeY, int enemyType, int enemyNumber, Render window, SDL_Texture *Tex, SDL_Texture *start1,SDL_Texture *gateC,SDL_Texture *gateO)
{
    mapTex = Tex;
    this->levelNumber = levelNumber;
    this->sizeX = sizeX;
    this->sizeY = sizeY;
    this->enemyType = enemyType;
    this->enemyNumber = enemyNumber;
    this->window = window;
    setupTex=start1;
    gateClosed=gateC;
    gateOpen=gateO;
    if(levelNumber==3){
       scientist = new Entity(3300,-1165,window.loadTexture("src/res/gfx/scientist.png"),true);
    }

    // Initialize srcRect and dstRect after setting sizeX and sizeY

    srcRect = {(sizeX - 1920) / 2, (sizeY - 1080) / 2, 1920, 1080};
    dstRect = {0, 0, 1920, 1080};
    createWalls();
    createEnemies();
    createClues();

    
}

void Level::render()
{
    window.renderTexture(mapTex, srcRect, dstRect);
    for (int i = 0; i < enemyNumber; ++i)
    {
        window.renderEntity(enemies.at(i), enemies.at(i).getX(), enemies.at(i).getY(), 100, 100, enemies.at(i).Alive());
    }
    for (int i = 0; i < clues.size(); i++)
    {
        window.renderEntity(clues.at(i), clues.at(i).getX(), clues.at(i).getY(), 100, 100, clues.at(i).Alive());
    }
    if(levelNumber==3){
        window.renderEntity(*scientist, scientist->getX(), scientist->getY(), 100, 100, scientist->Alive());
    }

    if(!levelComplete){
        window.renderTexture1(gateClosed, gateRectClosed);
    }
    else{
        window.renderTexture1(gateOpen,gateRectOpen);
    }
}


void Level::createEnemies()
{
    srand(time(NULL));
    SDL_Rect enemyTempRect;
    int EnemyX, EnemyY;
    bool collision;
    for (int i = 0; i < enemyNumber; i++)
    {
        
        do
        {   if(levelNumber==1){
            EnemyX = rand()% 2646 - 413; 
            EnemyY = rand()% 1440 - 227; 

        }
            else if(levelNumber==2){
            EnemyX = rand() %  4944 - 1555;
            EnemyY = rand() %  2727 - 873; 
        }
            else if(levelNumber==3){
            EnemyX = rand() %  7402-2791;
            EnemyY = rand() %  4120 - 1570; 
        }

            enemyTempRect = {EnemyX, EnemyY, 100, 100};
            collision = enemyCheckCollision(&enemyTempRect);
       
        } while (collision);

        if(levelNumber==1)
        enemies.push_back(Entity(EnemyX, EnemyY, window.loadTexture("src/res/gfx/ppl_textures/desert enemy/idle.png"), true));
        else if(levelNumber==2)
        enemies.push_back(Entity(EnemyX, EnemyY, window.loadTexture("src/res/gfx/ppl_textures/forest enemy/idle.png"), true));
        else if(levelNumber==3)
        enemies.push_back(Entity(EnemyX, EnemyY, window.loadTexture("src/res/gfx/ppl_textures/city enemy/idle.png"), true));
    }
}

Entity &Level::useEnemy(int i)
{
    return enemies.at(i);
}

vector<Entity> &Level::getEnemies()
{
    return enemies;
}

int Level::getEnemyNumber()
{
    return enemyNumber;
}

vector<SDL_Rect> &Level::getWalls()
{
    return walls;
}

void Level::resetLevel()
{
    enemies.clear();
    createEnemies();
}

void Level::moveAll(int x, int y)
{
    for (int i = 0; i < walls.size(); i++)
    {
        walls.at(i).x += x;
        walls.at(i).y += y;
    }
    for (int i = 0; i < deathBarriers.size(); i++)
    {
        deathBarriers.at(i).x += x;
        deathBarriers.at(i).y += y;
    }
    for (int i = 0; i < clues.size(); i++)
    {
        clues.at(i).Move(clues.at(i).getX() + x, clues.at(i).getY() + y);
    }
    for (int i = 0; i < enemyNumber; ++i)
     {
        enemies[i].Move(enemies[i].getX() + x, enemies[i].getY() + y);
     }
     if(levelNumber==3){
        scientist->Move(scientist->getX() + x, scientist->getY() + y);
     }

    gateRectClosed.x +=x;
    gateRectOpen.x +=x;
    gateRectOpen.y+=y;
    gateRectClosed.y+=y;

    srcRect.x -= x;
    srcRect.y -= y;

}

bool Level::checkCollision(SDL_Rect *playerHitbox)
{
    for (int i = 0; i < walls.size(); i++)
    {
        if (SDL_HasIntersection(playerHitbox, &walls.at(i)))
        {
            return true;
        }
        
    }
    if(SDL_HasIntersection(playerHitbox, &gateRectClosed))
        {
            return false;
        }
    return false;
}

void Level::createWalls()
{
    if (levelNumber == 1)
    {
        walls.push_back({-414, -233, 50, 650});
        walls.push_back({-414, 417, 650, 50});
        walls.push_back({186, 117, 50, 350});
        walls.push_back({186, -233, 50, 200});
        walls.push_back({1684, 613, 650, 50});
        walls.push_back({1684, 613, 50, 200});
        walls.push_back({1685, 963, 50, 350});
        walls.push_back({1684, 1263, 650, 50});
        walls.push_back({2284, 663, 50, 650});
        walls.push_back({-414, -233, 650, 60});
        walls.push_back({-369,982,222,89});

        deathBarriers.push_back({2231,-72,33,26});
        deathBarriers.push_back({2231,-233,77,161});
        deathBarriers.push_back({1663,-233,568,170});
        deathBarriers.push_back({1413,-233,250,43});

        gateRectOpen  = {2123,121,207,401};
        gateRectClosed  = {2314,121,31,414};
        }
    else if (levelNumber == 2)
    {
        walls.push_back({-1563, 879, 1229, 50});
        walls.push_back({1517, -879, 50, 968});
        walls.push_back({2607, -24, 50, 135});
        walls.push_back({1517, 64, 1140, 50});
        walls.push_back({-936, 1482, 50, 219});
        walls.push_back({-936, 1125, 50, 375});
        walls.push_back({2607, -879, 50, 603});
        walls.push_back({-384, 879, 50, 603});
        walls.push_back({-886, 1432, 502, 50});
        walls.push_back({-334, 1432, 502, 50});
        walls.push_back({-1563, 904, 50, 1055});
        walls.push_back({-1563, 1909, 1731, 50});
        walls.push_back({-1563, -879, 3080, 439});

        deathBarriers.push_back({-399, 461, 296,200});
        deathBarriers.push_back({-428,-879,645,855});
        deathBarriers.push_back({-1544, 521, 1116, 279});

        gateRectOpen  = {-1567,-234,234,460};
        gateRectClosed  = {-1586,-241,35,460};
    }
    else if(levelNumber==3){
        walls.push_back({-306,-1571,50,2431});
        walls.push_back({-2796, -1568,50,2431});
        walls.push_back({-2796,860,943,50});
        walls.push_back({-1134,860,878,50});
        walls.push_back({-1134,-954,878,50});
        walls.push_back({-1134,-338,878,50});
        walls.push_back({-1134,261,878,50});
        walls.push_back({-2796,261,878,50});
        walls.push_back({-2796,-338,878,50});
        walls.push_back({-2796,-954,878,50});
        walls.push_back({-1134,274,50,361});
        walls.push_back({-1968,274,50,361});
        walls.push_back({-1968,-338,50,361});
        walls.push_back({-1968,-954,50,361});
        walls.push_back({-1134,-954,50,361});
        walls.push_back({-1134,-338,50,361});
        walls.push_back({-2790,-1570,2539,50});
        walls.push_back({3153,-1570,1558,50});
        walls.push_back({3103,-1570,50,1418});
        walls.push_back({3153,-215,1286,50});
        walls.push_back({3858,-175,50,111});
        walls.push_back({3858,111,50,111});
        walls.push_back({3853,202,851,50});
        walls.push_back({4661,-1520,50,1747});
        walls.push_back({3209,1534,10,1096});
        walls.push_back({3212,998,1487,324});
        walls.push_back({-2309,-1351,1568,186});

        gateRectOpen = {3686,-900,56,197};
        gateRectClosed = {3668,-1139,56,197};


    }
}

bool Level::checkDeathCollision(SDL_Rect *playerHitbox)
{
    for (int i = 0; i < deathBarriers.size(); i++)
    {
        if (SDL_HasIntersection(playerHitbox, &deathBarriers.at(i)))
        {
            return false;
        }
    }
    return true;
}

bool Level::enemyCheckCollision(SDL_Rect *enemyHitbox)
{
    for (int i = 0; i < walls.size(); i++)
    {
        if (SDL_HasIntersection(enemyHitbox, &walls.at(i)))
        {
            return true;
        }
    }
    
    for (int i = 0; i < deathBarriers.size(); i++)
    {
        if (SDL_HasIntersection(enemyHitbox, &deathBarriers.at(i)))
        {
            return true;
        }
    }

 
    return false; // No collision
}

bool Level::checkComplete()
{
    levelComplete = true;
    for (int i = 0; i < enemies.size(); i++)
    {
        if (enemies.at(i).Alive())
        {
            levelComplete = false;
            goto return1;
        }
    }
    for (int i = 0; i < clues.size(); i++)
    {
        if (!clues.at(i).Alive())
        {
            levelComplete = false;
        }
    }
return1:
    return levelComplete;
}

void Level::createClues()
{
    if (levelNumber == 1)
    {
        clues.push_back(Clue(-250, -133, false, window));
        clues[0].SetClueType("question", "What colour is the star on the big crate?", "purple", "Look! The gate has opened!");
        clues.push_back(Clue(-376, 888,  false, window));
        clues[1].SetClueType("clue", "I should look in between the bushes.");
        clues.push_back(Clue(1490,-70,  false, window));
        clues[2].SetClueType("clue", "Maybe that last house has something?");
        clues.push_back(Clue(1793, 1112,  false, window));
        clues[3].SetClueType("clue", "That big rock on the left looks suspicious.");
    }
    else if(levelNumber==2){
        clues.push_back(Clue(-540,1215,false,window));
        clues[0].SetClueType("clue", "I should look for snakes like the one on this jar in the bushes.");
        clues.push_back(Clue(3028,1399,false,window));
        clues[1].SetClueType("clue", "What a big snake. I should continue my search in the house above me.");
        clues.push_back(Clue(-866,-190,false,window));
        clues[2].SetClueType("clue", "How dare they cut down this tree! At least the gate should be open now.");
        clues.push_back(Clue(1753,-791,false,window));
        clues[3].SetClueType("question", "How many coins are there on the floor?", "8", "Let me check across the river for more clues.");
        clues.push_back(Clue(619,629,false,window));
        clues[4].SetClueType("clue", "I should definitely check the house on the left.");
    }
}

bool Level::checkClues(Entity *player1)
{
 
    for (int i = 0; i < clues.size(); i++)
    {
        if (SDL_HasIntersection(player1->getHitbox(), clues.at(i).getHitbox()) && clues.at(i).Alive() == false)
        {
            
           clues.at(i).solve(*this);
            clues.at(i).setAlive(clues.at(i).getSolved());
            return false;
        }
    }
    return true;


 

}



void Level::start(){

    window.clear();
    window.renderTexture(setupTex,{0,0,1920,1080},dstRect);
    window.display();
    SDL_Event event;
    bool running=true;

    while(running){
        while(SDL_PollEvent(&event)){
            if(event.type=SDL_KEYDOWN){
                if(event.key.keysym.sym==SDLK_RETURN){
                    running=false;
                }
            }
        }
    }


}

SDL_Rect& Level::getgateRect() {
    if (!levelComplete) {
        return gateRectClosed;
    } else {
        return gateRectOpen;
    }
}



int Level::clueDistance() {
    int closest = 0; 

    for (int i = 0; i < clues.size(); i++) {
        if (!clues[i].Alive()) { 
            if (clues[i].Circle(clues[i].getX(), clues[i].getY(), 200)) {
                closest = std::max(closest, 5); 
            } else if (clues[i].Circle(clues[i].getX(), clues[i].getY(),350)) {
                closest = std::max(closest, 4);
            } else if (clues[i].Circle(clues[i].getX(), clues[i].getY(), 500)) {
                closest = std::max(closest, 3);
            } else if (clues[i].Circle(clues[i].getX(), clues[i].getY(), 650)) {
                closest = std::max(closest, 2);
            } else if (clues[i].Circle(clues[i].getX(), clues[i].getY(), 800)) {
                closest = std::max(closest, 1);
            }
        }
    }
    

    return closest; 
}

void Level::loadPlayer(Entity &x){
    Player temp(&x);
    player1=temp;
   

    
}

vector<Clue>& Level::getClues()
{
    return clues;
}

void Level::loadFromFile(const std::string& filename, Render &window) {
    SDL_Texture* skins[3];
    skins[0] = window.loadTexture("src/res/gfx/ppl_textures/desert enemy/idle.png");
    skins[1] = window.loadTexture("src/res/gfx/ppl_textures/forest enemy/idle.png");
    skins[2] = window.loadTexture("src/res/gfx/ppl_textures/city enemy/idle.png");

    std::ifstream inFile("level.bin", std::ios::binary);
    if (!inFile) {
        std::cerr << "Error opening file for reading!" << std::endl;
        return;
    }

    // Read the level number
    inFile.read(reinterpret_cast<char*>(&levelNumber), sizeof(levelNumber));

    // Read clue states
    size_t clueSize;
    inFile.read(reinterpret_cast<char*>(&clueSize), sizeof(clueSize));

    std::vector<uint8_t> clueData(clueSize);
    inFile.read(reinterpret_cast<char*>(clueData.data()), clueSize);

    std::vector<bool> clueStates(clueSize);
    for (size_t i = 0; i < clueSize; i++) {
        clueStates[i] = (clueData[i] != 0); // Convert byte to bool
    }

    // Read enemy alive states
    size_t enemySize;
    inFile.read(reinterpret_cast<char*>(&enemySize), sizeof(enemySize));

    std::vector<uint8_t> enemyData(enemySize);
    inFile.read(reinterpret_cast<char*>(enemyData.data()), enemySize);

    std::vector<bool> enemyStates(enemySize);
    for (size_t i = 0; i < enemySize; i++) {
        enemyStates[i] = (enemyData[i] != 0); // Convert byte to bool
    }

    // Read player health
    int helt;
    inFile.read(reinterpret_cast<char*>(&helt), sizeof(helt));
    player1.setHealth(helt);

    inFile.close();

    // Apply loaded data
    for (size_t i = 0; i < clues.size() && i < clueStates.size(); i++) {
        clues[i].setAlive(clueStates[i]);
    }

    for (size_t i = 0; i < enemies.size() && i < enemyStates.size(); i++) {
        enemies[i].setAlive(enemyStates[i]);
    }

   
}






void Level::saveToFile(const std::string& filename) {
    std::ofstream outFile("level.bin", std::ios::binary);
    if (!outFile) {
        std::cerr << "Error opening file for writing!" << std::endl;
        return;
    }

    // Save the level number
    outFile.write(reinterpret_cast<const char*>(&levelNumber), sizeof(levelNumber));

    // Save the clue data
    std::vector<bool> clueStates;
    for (size_t i = 0; i < clues.size(); i++) {
        clueStates.push_back(clues[i].Alive());
    }
    size_t clueSize = clueStates.size();
    outFile.write(reinterpret_cast<const char*>(&clueSize), sizeof(clueSize));

    std::vector<uint8_t> clueData(clueSize);
    for (size_t i = 0; i < clueSize; i++) {
        clueData[i] = clueStates[i] ? 1 : 0; // Convert bool to byte
    }
    outFile.write(reinterpret_cast<const char*>(clueData.data()), clueSize);

    // Save the enemy alive states
    std::vector<bool> enemyStates;
    for (size_t i = 0; i < enemies.size(); i++) {
        enemyStates.push_back(enemies[i].Alive());
    }
    size_t enemySize = enemyStates.size();
    outFile.write(reinterpret_cast<const char*>(&enemySize), sizeof(enemySize));

    std::vector<uint8_t> enemyData(enemySize);
    for (size_t i = 0; i < enemySize; i++) {
        enemyData[i] = enemyStates[i] ? 1 : 0; // Convert bool to byte
    }
    outFile.write(reinterpret_cast<const char*>(enemyData.data()), enemySize);

    // Save player health
    int helt = player1.getHealth();
    outFile.write(reinterpret_cast<const char*>(&helt), sizeof(helt));

    outFile.close();
    std::cout << "Level saved successfully!" << std::endl;
}


int Level::numberCheck() {
    std::ifstream inFile("level.bin", std::ios::binary);
    if (!inFile) {
        std::cerr << "Error opening file for reading!" << std::endl;
        return -1;
    }

    // Read the level number into a temporary variable
    int tempLevelNumber;
    inFile.read(reinterpret_cast<char*>(&tempLevelNumber), sizeof(tempLevelNumber));

    inFile.close();

    // Return the temporary level number
    return tempLevelNumber;
}



void Level::renderCounter(Render &window){
    int enemiesLeft=0;
    for(int i=0;i<enemies.size();i++){
        if(enemies[i].Alive()){
            enemiesLeft++;
        }
    }
    Text enemyText = Text(window.getRenderer(), {0, 0, 0, 255}, TTF_OpenFont("src/res/dev/IBMPlexMono-Medium.ttf", 70), "Enemies Left: " + std::to_string(enemiesLeft), 0,0, 60, 400,{255,255,255,75});

    int cluesLeft=0;
    for(int i=0;i<clues.size();i++){
        if(!clues[i].Alive()){
            cluesLeft++;
        }
    }
    Text clueText = Text(window.getRenderer(), {0,0, 0, 255}, TTF_OpenFont("src/res/dev/IBMPlexMono-Medium.ttf", 70), "Clues Left: " + std::to_string(cluesLeft), 0,0, 60, 400,{255,255,255,75});

    enemyText.renderText(0,00);
    clueText.renderText(0,60);
}

