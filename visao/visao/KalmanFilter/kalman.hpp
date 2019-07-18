#ifndef KALMANFILTER2D_H

#define KALMANFILTER2D_H

#include <QString>

#include "include/timer.h"
#include "types/types.h"
#include "kalmanstate.h"
#include "matrix.h"

class KalmanFilter2D {

private:

    Timer _timer;

    // Initial state

    bool _has1stPosition, _has2ndPosition;

    // State and covariance matrices

    KalmanState _X, _Y;

    Matrix _Px, _Py;

    // Model

    Matrix _A;
    Matrix _H;
    Matrix _Q;
    Matrix _R;

    // Model config

    static constexpr float _p = 1;
    static constexpr float _sigma_a = 0.01; // affects Q
    static constexpr float _r = 0.000283; // affects R

    // Private methods

    void updateMatrices(const float T);
    bool enabled;

public:

    KalmanFilter2D();

    QString name();

    void iterate(Position &pos);

    void predict();

    Position getPosition();

    Velocity getVelocity();

    Velocity getAcceleration();

    void setEnabled(bool _enable);

    bool getEnabled();

};
#endif
