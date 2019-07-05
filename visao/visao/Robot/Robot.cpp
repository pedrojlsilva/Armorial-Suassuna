#include "Robot.hpp"

/* Robot methods */

Robot::Robot(uint8_t id, bool teamBlue){
    this->robot_id = id;
    this->active = true;
    this->teamBlue = teamBlue;
}

Robot::~Robot(){
    this->active = false;
}

void Robot::printRobotInfo(){
    if(this->getTeam() == 0) printf("Yellow Robot |");
    else printf("Blue Robot |");

    printf("ID: %d\t", this->robot_id);
    printf("Altura: %6.2lf | Posicao: <%9.2lf,%9.2lf |", this->getHeight(), this->getRobotX(), this->getRobotY());
    printf("Angulo: %6.2lf |\n", this->getAngle());
}

void Robot::setCoordinates(double coordX, double coordY){
    this->coordX = coordX;
    this->coordY = coordY;
}

void Robot::setId(uint8_t id){
    this->robot_id = id;
}

void Robot::setAngle(double angle){
    this->angle = angle;
}

void Robot::setHeight(double height){
    this->height = height;
}

double Robot::getRobotX(){
    return this->coordX;
}

double Robot::getRobotY(){
    return this->coordY;
}

double Robot::getHeight(){
    return this->height;
}

double Robot::getAngle(){
    return this->angle;
}

bool Robot::isActive(){
    return this->active;
}

bool Robot::getTeam(){
    return this->teamBlue;
}

/* end of Robot methods */