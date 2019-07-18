#include "kalman.hpp"
#include "cmath"
#include "types/types.h"

QString KalmanFilter2D::name() {
    return "KalmanFilter2D";
}

KalmanFilter2D::KalmanFilter2D() {

    _has1stPosition = _has2ndPosition = false;
    enable=false;

    // Initialize state matrices

    _Px = Matrix::diag(3, KalmanFilter2D::_p);

    _Py = Matrix::diag(3, KalmanFilter2D::_p);

    // Initialize model matrices

    _A = Matrix::identity(3);

    _H.setSize(1, 3);

    _H.set(0, 0, 1);

    _Q.setSize(3, 3);

    _R.setSize(1, 1);

    _R.set(0, 0, pow(KalmanFilter2D::_r, 2));

    _timer.start();

}

void KalmanFilter2D::updateMatrices(const float T) {

    // Update A with time (T)

    _A.set(0, 1, T);

    _A.set(0, 2, pow(T,2)/2);

    _A.set(1, 2, T);

    /// Time update / PREDICT

    // Project state ahead

    _X.matrix() = _A*_X.matrix();

    _Y.matrix() = _A*_Y.matrix();

    // Update Q with time (T)

    _Q.set(0, 0, pow(T,4)/4);

    _Q.set(0, 1, pow(T,3)/2);

    _Q.set(0, 2, pow(T,2)/2);

    _Q.set(1, 0, pow(T,3)/2);

    _Q.set(1, 1, pow(T,2));

    _Q.set(1, 2, T);

    _Q.set(2, 0, pow(T,2)/2);

    _Q.set(2, 1, T);

    _Q.set(2, 2, 1);

    _Q *= pow(KalmanFilter2D::_sigma_a, 2);

    // Project the error covariance ahead

    _Px = _A*_Px*_A.transposed() + _Q;

    _Py = _A*_Py*_A.transposed() + _Q;

}

void KalmanFilter2D::iterate(Position &pos) {

    // Check if position is known

    if(pos.isUnknown())

        return this->predict();

    // Get iteration time

    _timer.stop();

    const float T = _timer.timesec();

    _timer.start();

    // Initial states

    if(_has1stPosition==false) {

        _X.setPosition(pos.getX());

        _Y.setPosition(pos.getY());

        _has1stPosition = true;

        return;

    }

    if(_has2ndPosition==false) {

        _X.setVelocity((pos.getX() - _X.getPosition())/T);

        _X.setPosition(pos.getY());

        _Y.setVelocity((pos.getY() - _Y.getPosition())/T);

        _Y.setPosition(pos.getY());

        _has2ndPosition = true;

        return;

    }

    updateMatrices(T);

    /// Measurement update / CORRECT

    // Compute the Kalman gain

    Matrix Kx = _Px*_H.transposed() * (1/(_H*_Px*_H.transposed() + _R).get(0, 0));

    Matrix Ky = _Py*_H.transposed() * (1/(_H*_Py*_H.transposed() + _R).get(0, 0));

    // Update estimate with measurement Zk

    _X.matrix() = _X.matrix() + Kx*(pos.getX() - (_H*_X.matrix()).get(0, 0));

    _Y.matrix() = _Y.matrix() + Ky*(pos.getY() - (_H*_Y.matrix()).get(0, 0));

    // Update the error covariance

    const Matrix I = Matrix::identity(3);

    _Px = (I - Kx*_H)*_Px;

    _Py = (I - Ky*_H)*_Py;

}

void KalmanFilter2D::predict() {

    // Get iteration time

    _timer.stop();

    const float T = _timer.timemsec()*1E-3;

    _timer.start();

    // Check initial states, if do not have, quit. cannot make prevision...

    if(_has1stPosition==false || _has2ndPosition==false) {

        return;

    }

    updateMatrices(T);

}

void KalmanFilter2D::setEnabled(bool _enable){
    enabled=_enable;
}

bool KalmanFilter2D::getEnabled(){
    return enabled;
}


Position KalmanFilter2D::getPosition() {

    return Position(true, _X.getPosition(), _Y.getPosition());

}

Velocity KalmanFilter2D::getVelocity(){

   return Velocity(true, _X.getVelocity(), _Y.getVelocity());

}

Velocity KalmanFilter2D::getAcceleration(){

    return Velocity(true, _X.getAcceleration(), _Y.getAcceleration());

}
