#ifndef KALMANFILTER_H
#define KALMANFILTER_H

#include "kalmanState.h"
#include "matrix.h"

class KalmanFilter2D{


    private:

    //Matrizes de estado e de covariancia para X e para Y
    KalmanState _stateX, _stateY;


    public:
    KalmanFilter();

}

#endif