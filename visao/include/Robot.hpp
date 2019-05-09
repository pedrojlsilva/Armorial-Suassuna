#pragma once
#include <stdint.h>

class Robot{
    public:
        Robot(uint8_t, bool);
        ~Robot();
        void printRobotInfo();
        void setCoordinates(double, double);
        void setAngle(double);
        void setHeight(double);
        double getRobotX();
        double getRobotY();
        double getHeight();
        double getAngle();
        void setId(uint8_t);
        bool isActive();
        bool getTeam();
   		uint8_t robot_id;
   	
    private:
        bool active;
        bool teamBlue;
        double height, angle;
        double coordX, coordY;

};