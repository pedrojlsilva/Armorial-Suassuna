#include "frame.h"

Frame::Frame()
{

}

void Frame::setFrame(char qt_blue, char qt_yellow, char camera_id, std::vector<Robot> blueRobots, std::vector<Robot> yellowRobots, Ball ball)
{
    _qt_blue = qt_blue;
    _qt_yellow = qt_yellow;
    _blueRobots = blueRobots;
    _yellowRobots = yellowRobots;
    _camera_id = camera_id;
    _ball = ball;
}

char Frame::qt_blue() const {
    return _qt_blue;
}

char Frame::qt_yellow() const {
    return _qt_yellow;
}

char Frame::camera_id() const {
    return _camera_id;
}

std::vector<Robot> Frame::blueRobots() const {
    return _blueRobots;
}

std::vector<Robot> Frame::yellowRobots() const {
    return _yellowRobots;
}

Ball Frame::ball() const {
    return _ball;
}







