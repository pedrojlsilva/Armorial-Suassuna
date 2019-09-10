#include "object.h"

Object::Object(bool enableLossFilter, bool enableKalmanFilter, bool enableNoiseFilter) {

//    _lossFilter.setEnabled(enableLossFilter);

    _kalmanFilter.setEnabled(enableKalmanFilter);

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

    _mutex->lockForRead();

    Position retn = _position;

   // _mutex.unlock();

    return retn;

}

Velocity Object::getVelocity() {

    //_mutex.lockForRead();

    Velocity retn = _velocity;

   // _mutex.unlock();

    return retn;

}

Angle Object::getOrientation() {

   // _mutex.lockForRead();

    Angle retn = _orientation;

   // _mutex.unlock();

    return retn;

}

void Object::setFilterTimes(double time){
    this->_lossFilter.setFilterTime(time);
    this->_noiseFilter.setFilterTime(time);
}

double Object::getConfidence() {

    //_mutex.lockForRead();

    double retn = _confidence;

   // _mutex.unlock();

    return retn;

}

void Object::update(double confidence, Position pos, Angle ori) {

   // _mutex.lockForWrite();
    
    _confidence = confidence;

    if(!_noiseFilter.isInitialized()){
        _noiseFilter.initCounter();
        _position.setInvalid(); // como o filtro de ruido ainda nao terminou
                                // é necessário invalidar a posição até que ele acabe
    }else{
        if(_noiseFilter.noiseFilter()){ // caso o filtro tenha terminado o tempo
            _lossFilter.lossFilter(true); // dou update no filtro de perda
            _kalmanFilter.iterate(pos); // inicializa mais uma iteração no kalman
            _position.setPosition(_kalmanFilter.getPosition().getX(), _kalmanFilter.getPosition().getY()); // pega a posição retornada pelo kalman
            _orientation.setValue(ori.value()); // dou update no angulo do robo
        }
    }
    //_mutex.unlock();

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
