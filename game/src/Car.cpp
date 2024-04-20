#include "Car.h"
#include "cmath"
# define PI 3.14159265358979323846

Car::Car(float startX, float startY)
{
    this->x = startX;
    this->y = startY;
}

void Car::accelerate(float frameTime){
    this->direction = -1;
    this->speed += this->speedInterval * frameTime;
    if(this->speed >= this->max_speed) {
        this->speed = this->max_speed;
    }
}

void Car::deaccelerate(float frameTime){
    this->direction = 1;
    this->speed -= this->speedInterval * frameTime;
    if(this->speed >= this->max_speed) {
        this->speed = this->max_speed;
    }
}

void Car::slowUntilStationary(float frameTime){
    this->speed += this->slowInterval * frameTime * this->direction;

    if(this->direction == -1 && this->speed < 0) {
        this->speed = 0;
        return;
    }
    
    if(this->direction == 1 && this->speed > 0) {
        this->speed = 0;
        return;
    }
}

void Car::rotate(float frameTime, int rotationDirection){
    if(rotationDirection != 1 && rotationDirection != -1){
        return;
    }

    this->rotation += this->rotationSpeed * frameTime * abs(this->speed) * rotationDirection * this->direction;
    if(this->rotation >= 360) {
        this->rotation = 0;
    }
}

void Car::move(){
    float radians = PI * this->rotation / 180;
    float x_move = this->speed * cosf(radians);
    float y_move = this->speed * sinf(radians);

    this->x += x_move;
    this->y += y_move;
}

void Car::reinstate(float prevX, float prevY){
    this->x = prevX;
    this->y = prevY;
}

float Car::getX(){
    return this->x;
}

float Car::getY(){
    return this->y;
}

float Car::getRotation(){
    return this->rotation;
}