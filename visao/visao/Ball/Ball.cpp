
#include "Ball.hpp"

Ball::Ball(){
    this->active = true;
    this->coordX = 0;
    this->coordY = 0;
}   

Ball::~Ball(){
    this->active = false;
    this->coordX = 0;
    this->coordY = 0;
}

void Ball::setCoordinates(double coordX, double coordY){
    this->coordX = coordX;
    this->coordY = coordY;
}

double Ball::getBallX(){
    return this->coordX;
}

double Ball::getBallY(){
    return this->coordY;
}

void Ball::printBallInfo(){
    printf("Bola\tPosicao: <%9.2lf, %9.2lf>\n", this->getBallX(), this->getBallY());
}

bool Ball::isActive(){
    return this->active;
}

/* end of Ball methods */