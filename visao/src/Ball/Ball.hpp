#pragma once

class Ball{
    public:
        Ball();
        ~Ball();
        void setCoordinates(double, double);
        double getBallX();
        double getBallY();
        void printBallInfo();
        bool isActive();

    private:
        bool active;
        double coordX, coordY;

};