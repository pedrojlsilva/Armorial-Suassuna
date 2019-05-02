#include "Robot.hpp"

Robot::Robot(bool isTeamYellow, uint8_t numberId) {
    this->teamYellow = isTeamYellow;
    this->id = numberId;
    this->isActive = false;
    this->coordX = 0;
    this->coordY = 0;
    this->coordW = 0;
}

Robot::~Robot() {
    this->isActive=false;
    this->coordX = 0;
    this->coordY = 0;
    this->coordW = 0;

}

void Robot::setIsActive(bool isTeamYellow){
    this->isActive=isTeamYellow;
}
void Robot::setAllCoordinates(float newCoordX, float newCoordY, float newCoordW){
    this->coordX = newCoordX;
    this->coordY = newCoordY;
    this->coordW = newCoordW;

}