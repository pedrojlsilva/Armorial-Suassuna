#ifndef FRAME_H
#define FRAME_H

#include <vector>
#include "Ball/ball.h"
#include "Robot/robot.h"



class Frame
{
public:
    Frame();
    void setFrame(char qt_blue, char qt_yellow, char camera_id, std::vector<Robot> blueRobots, std::vector<Robot> yellowRobots, Ball ball);



    char qt_blue() const;
    char qt_yellow() const;
    char camera_id() const;
    std::vector<Robot> blueRobots() const;
    std::vector<Robot> yellowRobots() const;
    Ball ball() const;

private:
    char _qt_blue;
    char _qt_yellow;
    char _camera_id;
    std::vector<Robot> _blueRobots;
    std::vector<Robot> _yellowRobots;
    Ball _ball;
};

#endif // FRAME_H
