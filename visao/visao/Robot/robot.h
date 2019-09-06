#ifndef ROBOT_H
#define ROBOT_H


#include <types/object.h>

class Robot:public Object{

private:

    // Robot info

    quint8 _teamId;
    quint8 _robotId;
    bool _debugDetection;
    quint8 _baterry;



public:
    enum Color
    {
        YELLOW,
        BLUE
    };

    quint8 _color;

    Robot(quint8 color, quint8 teamId, quint8 robotId, bool enableLossFilter=true, bool enableKalmanFilter=true, bool enableNoiseFilter=true, bool debugDetection=true);
    virtual ~Robot();

    quint8 color() const { return _color; }
    quint8 teamId() const { return _teamId; }
    quint8 robotId() const { return _robotId; }
    Position getRobotPosition();
    Velocity getRobotVelocity();

    QString name();

    //void updateToSensor();

};

#endif // ROBOT_H
