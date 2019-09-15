#include "robot.h"


/* Robot methods */

int Robot::getBattery() const
{
    return _battery;
}

void Robot::setBattery(int battery)
{
    _battery = battery;
}

Robot::Robot(quint8 color, quint32 teamId, quint32 robotId, bool enableLossFilter, bool enableKalmanFilter, bool enableNoiseFilter, bool debugDetection) :  Object(enableLossFilter, enableKalmanFilter, enableNoiseFilter)
{

    _color = color;
    _teamId = teamId;
    _robotId = robotId;
    _debugDetection = debugDetection;

}

QString Robot::name() {

    QString ss;

    ss = QString(_color==0?"Blue":"Yellow") + " robot, id #" + QString::number(_robotId);

    return QString(ss);

}



Robot::~Robot() {
    _color = 0;
    _teamId = 0;
    _robotId = 0;
}

/* end of Robot methods */