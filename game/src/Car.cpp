#include "Car.h"
#include <cmath>
#include <cstdlib>
#define PI 3.14159265358979323846

Car::Car(float startX, float startY)
{
    this->x = startX;
    this->y = startY;
}

void Car::accelerate(float frameTime){
    this->direction = -1;
    this->speed += this->speedInterval * frameTime;
    this->rotate(frameTime, (rand() % 2));
    if(this->speed >= this->max_speed) {
        this->speed = this->max_speed;
    }
}

void Car::deaccelerate(float frameTime){
    this->direction = 1;
    this->speed -= this->speedInterval * frameTime;
    this->rotate(frameTime, (rand() % 2));
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

    this->rotation += this->rotationSpeed * frameTime * abs(this->speed) * rotationDirection * this->direction * (rand() % 1001) / 350;
    if(this->rotation >= 360) {
        this->rotation = 0;
    }
    if(this->rotation < 0){
        this->rotation = abs(360-this->rotation);
    }
}

void Car::move(){
    float radians = PI * this->rotation / 180;
    float x_move = this->speed * cosf(radians);
    float y_move = this->speed * sinf(radians);

    this->x += x_move;
    this->y += y_move;
}

void Car::reinstate(float prevX, float prevY, float prevR){
    this->x = prevX;
    this->y = prevY;
    this->rotation = prevR;
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