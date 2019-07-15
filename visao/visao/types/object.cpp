#include "object.h"


Object::Object(bool enableLossFilter, bool enableKalmanFilter, bool enableNoiseFilter) {

    _lossFilter.setEnabled(enableLossFilter);

    _kalmanFilter.setEnabled(enableKalmanFilter);

    _noiseFilter.setEnabled(enableNoiseFilter);

    _zero = false;

    _position.setUnknown();

    _velocity.setUnknown();

    _orientation.setUnknown();

    _confidence = 0.0;

}

Object::~Object() {

}

Position Object::position() {

    _mutex.lockForRead();

    Position retn = _position;

    _mutex.unlock();

    return retn;

}

Velocity Object::velocity() {

    _mutex.lockForRead();

    Velocity retn = _velocity;

    _mutex.unlock();

    return retn;

}

Angle Object::orientation() {

    _mutex.lockForRead();

    Angle retn = _orientation;

    _mutex.unlock();

    return retn;

}

double Object::confidence() {

    _mutex.lockForRead();

    double retn = _confidence;

    _mutex.unlock();

    return retn;

}

void Object::update(double confidence, const Position &pos, const Angle &ori) {

    _mutex.lockForWrite();

    // Update confidence

    _confidence = confidence;

    // Object lost

    if(pos.isUnknown()) {

        // Set to unknown if object is really lost

        if(_lossFilter.enabled()==false || (_lossFilter.enabled() && _lossFilter.isObjectLost())) {

            // Reset object safe

            _noiseFilter.resetNoiseTimer();

            setUnknown();

        // Maintain position/prediction for a little while before removing object

        } else {

            _zero = false;

            if(_kalmanFilter.enabled()) {

                // Predict trajetory with Kalman and keep same orientation

                _kalmanFilter.predict();

                _position = _kalmanFilter.getPosition();

                _velocity = _kalmanFilter.getVelocity();

                updateToSensor();

            } else {

                // Nothing, just leave robot where it disappeared

            }

        }

    // Object in camera

    } else {

        // Set position from camera if object is really safe

        if(_noiseFilter.enabled()==false || (_noiseFilter.enabled() && _noiseFilter.isObjectSafe())) {

            // Reset object lost

            _zero = false;

            _lossFilter.resetLossTimer();

            // Update

            if(_kalmanFilter.enabled()) {

                // Filter with Kalman

                _kalmanFilter.iterate(pos);

                _position = _kalmanFilter.getPosition();

                _velocity = _kalmanFilter.getVelocity();

                _orientation = ori;

                //updateToSensor();

            } else {

                // Just use brute data

                _position = pos;

                //_velocity = _objVel.getVelocity(pos);

                _orientation = ori;

                //updateToSensor();

            }

        // Maintain object unknown for a little while before including object

        } else {

            setUnknown();

        }

    }

    _mutex.unlock();

}

void Object::setUnknown() {

    if(_zero==false) {

        _confidence = 0.0;

        _position = Position(false, 0, 0, 0);

        _velocity = Velocity(false, 0, 0);

        _orientation = Angle(false, 0);

        _zero = true;

        //updateToSensor();

    }

}
