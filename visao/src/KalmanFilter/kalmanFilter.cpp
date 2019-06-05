#include "kalmanFilter.hpp"

class KalmanFilter{


    float Times;

    Matrix3f _Px;
    Matrix3f _Py;
    Matrix3f _A;
    MatrixXd _H;
    Matrix3f _Q;
    Matrix3f _R;

    public:
    MatrixXf _X;
    MatrixXf _Y;
    MatrixXf _Zx;
    MatrixXf _Zy;

    float r;

    KalmanFilter(){

        _Px= Eigen::Matrix3f::Identity(3, 3);
        _Py = _Px;

        _A = _Px;


        _H=Eigen::MatrixXd::Zero(1, 3);

        _R.resize(1,1);

        _R(0,0) = r;

        _H.resize(1,3);

        _H(0,0) = 1;

        _X.resize(3,1);
        _Y.resize(3,1);

        _Zx.resize(3,1);
        _Zy.resize(3,1);





    }

    void predict(){


        _X = _A*_X;
        _Y = _A*_Y;


        _Px = _A*_Px*_A.transpose() + _Q;
        _Py = _A*_Py*_A.transpose() + _Q;

        
    }

    void correct(){

        MatrixXf Kx = _Px*_H*((_H*_Px*_H.transpose()+_R).inverse());
        MatrixXf Ky = _Py*_H*((_H*_Py*_H.transpose()+_R).inverse());

        _X = _X + Kx *(_Zx - _H*_X);
        _Y = _Y + Ky *(_Zy - _H*_Y);

        _Py =  (MatrixXd::Identity(3, 3)  -  Ky*_H)*_Py;
        _Px =  (MatrixXd::Identity(3, 3)  -  Kx*_H)*_Px;





    }
    

};



