#include "Level.h"

Level::Level(float spawnX, float spawnY, float spawnR)
{
    this->spawnX = spawnX;
    this->spawnY = spawnY;
    this->spawnR = spawnR;
}

void Level::setStars(int star){
    if(star > 3 || star < 0) return;
    this->stars = star;
}

int Level::getStars(){
    return this->stars;
}