#pragma once
#include <stdint.h>

class Robot {


   
public:
    Robot(bool teamBlue,uint8_t id);
    ~Robot();
    void setIsActive(bool isTeamYellow);
    void setAllCoordinates(float newCoordX, float newCoordY, float newCoordW);




protected:


private:
    bool teamYellow;
    uint8_t id;
    bool isActive;
    float coordX;
    float coordY;
    float coordW;

};