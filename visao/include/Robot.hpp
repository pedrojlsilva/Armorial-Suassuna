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
        bool isActive();
        bool getTeam();
   
    private:
        uint8_t robot_id;
        bool active;
        bool teamBlue;
        double height, angle;
        double coordX, coordY;

};
