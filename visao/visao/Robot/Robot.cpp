#include "Robot.h"


/* Robot methods */

Robot::Robot(Color color, quint8 teamId, quint8 robotId, bool debugDetection) : Object() {

    _color = color;
    _teamId = teamId;
    _robotId = robotId;
    _debugDetection = debugDetection;

}

QString Robot::name() {

    QString ss;

    ss = QString(_color==Color::BLUE?"Blue":"Yellow") + " robot, id #" + QString::number(_robotId);

    return QString(ss);

}



Robot::~Robot() {

}

/* end of Robot methods */
