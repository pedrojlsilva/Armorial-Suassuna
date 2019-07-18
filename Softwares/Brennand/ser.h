#ifndef SER_H
#define SER_H

#include <QObject>

class ser : public QObject{
    Q_OBJECT

signals:
    void transmitindo(int);
    void procurando();
    void joystickA();
    void joystickB();
    void joystickY();
    void joystickX();
};

#endif
