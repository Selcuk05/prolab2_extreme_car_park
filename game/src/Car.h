#ifndef CAR_H
#define CAR_H

class Car
{   
    private:
        float max_speed = 5;
        float speedInterval = 3;
        float slowInterval = 15;
        float rotationSpeed = 15;
        float speed = 0;
        float x;
        float y;
        int direction = -1;
        float rotation = -90;
    public:
        Car(float startX, float startY);

        void accelerate(float frameTime);
        void deaccelerate(float frameTime);
        void slowUntilStationary(float frameTime);
        void rotate(float frameTime, int rotationDirection);
        void move();
        void reinstate(float prevX, float prevY);

        float getX();
        float getY();
        float getRotation();
};

#endif