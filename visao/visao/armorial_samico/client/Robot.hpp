#pragma once
#include <stdint.h>

class Robot{
    public:
        Robot(uint8_t, bool);
        ~Robot();
        uint8_t getId();
        void printRobotInfo();
        void setCoordinates(double, double);
        void setAngle(double);
        void setHeight(double);
        void setTeam(bool);
        double getRobotX();
        double getRobotY();
        double getHeight();
        double getAngle();
        void setId(uint8_t);
        bool isActive();
        bool getTeam();
    
    private:
        bool active;
        bool teamBlue;
        double height, angle;
        double coordX, coordY;
        uint8_t robot_id;

};

/* Robot methods */

Robot::Robot(uint8_t id, bool teamBlue){
    this->robot_id = id;
    this->active = true;
    this->teamBlue = teamBlue;
}

Robot::~Robot(){
    this->active = false;
}

uint8_t Robot::getId(){
    return this->robot_id;
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

void Robot::setTeam(bool teamBlue){
    this->teamBlue = teamBlue;
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