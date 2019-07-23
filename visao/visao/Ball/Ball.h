#ifndef BALL_H
#define BALL_H


#include <types/object.h>


class Ball : public Object {

private:

    bool _debugDetection;

public:

    Ball(bool debugDetection=true);

    QString name();

    Position getBallPosition();
    Velocity getBallVelocity();

    void updateToSensor();

};


#endif
