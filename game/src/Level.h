#ifndef LEVEL_H
#define LEVEL_H

class Level
{
    private:
        int stars = 0; 
    public:
        Level(float spawnX, float spawnY, float spawnR);
        void setStars(int star);
        int getStars();
        float spawnX;
        float spawnY;
        float spawnR;
};

#endif