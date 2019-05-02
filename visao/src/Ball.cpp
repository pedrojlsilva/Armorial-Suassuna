#include "Ball.hpp"

Ball::Ball() {
    this->active=true;
}

Ball::~Ball() {
    this->active=false;
}

void Ball::setBallPosition(float coordX, float coordY){
    this->coordX=coordX;
    this->coordY=coordY;
}

float Ball::getBallX(){
    return coordX;
}
float Ball::getBallY(){
    return coordY;
}

bool Ball::isActive(){
    return active;
}