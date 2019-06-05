#include<stdio.h>
#include<Eigen/Dense>

using Eigen::MatrixXd;
using Eigen::Matrix3f;
using Eigen::MatrixXf;
class KalmanFilter{

    MatrixXd _Px;
    MatrixXd _Py;
    MatrixXd _A;
    MatrixXd _H;
    MatrixXd _Q;
    MatrixXd _R;

    MatrixXd States;

    KalmanFilter();

    void predict();

    void correct();
    


};