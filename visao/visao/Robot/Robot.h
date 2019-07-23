#ifndef ROBOT_H
#define ROBOT_H


#include <types/object.h>

class Robot : public Object {

private:

    // Robot info

    quint8 _teamId;
    quint8 _robotId;
    bool _debugDetection;

    enum Color
    {
        YELLOW,
        BLUE
    };

    Color _color;

public:

    Robot(Color color, quint8 teamId, quint8 robotId, bool debugDetection=true);

    virtual ~Robot();

    Color color() const { return _color; }
    quint8 teamId() const { return _teamId; }
    quint8 robotId() const { return _robotId; }
    Position getRobotPosition();
    Velocity getRobotVelocity();

    QString name();

    //void updateToSensor();

};

#endif // ROBOT_H
