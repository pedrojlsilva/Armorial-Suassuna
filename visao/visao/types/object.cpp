#include "object.h"


Object::Object() {

//    _lossFilter.setEnabled(enableLossFilter);

    _kalmanFilter.setEnabled(true);

//    _noiseFilter.setEnabled(enableNoiseFilter);

    _zero = false;

    _position.setUnknown();

    _velocity.setUnknown();

    _orientation.setUnknown();

    _confidence = 0.0;

}

Object::~Object() {

}

Position Object::getPosition() {

    _mutex.lockForRead();

    Position retn = _position;

    _mutex.unlock();

    return retn;

}

Velocity Object::getVelocity() {

    _mutex.lockForRead();

    Velocity retn = _velocity;

    _mutex.unlock();

    return retn;

}

Angle Object::getOrientation() {

    _mutex.lockForRead();

    Angle retn = _orientation;

    _mutex.unlock();

    return retn;

}

double Object::getConfidence() {

    _mutex.lockForRead();

    double retn = _confidence;

    _mutex.unlock();

    return retn;

}

void Object::update(double confidence, const Position &pos, const Angle &ori) {

    _mutex.lockForWrite();

//atualizar com a lógica de zilde

    _mutex.unlock();

}

void Object::setUnknown() {

    if(_zero==false) {

        _confidence = 0.0;

        _position = Position(false, 0, 0);

        _velocity = Velocity(false, 0, 0);

        _orientation = Angle(false, 0);

        _zero = true;

        //updateToSensor();

    }

}
