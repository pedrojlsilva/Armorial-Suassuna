#pragma once

class Ball {

public:
    Ball();
    ~Ball();
    void setBallPosition(float coordX, float coordY);
    float getBallX();
    float getBallY();
    bool Ball::isActive();


protected:

private:
    float coordX;
    float coordY;
    bool active;
};