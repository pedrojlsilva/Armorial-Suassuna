#ifndef OBJECT_H
#define OBJECT_H


#include <QReadWriteLock>
#include "types/types.h"
#include "include/filters.h"
#include <QReadWriteLock>


class Object
{

private:
    bool enableKalman = true;
    bool enableLoss = true;
    bool enableNoise = true;
    double _confidence;

    // Thread-safe mutex
    QReadWriteLock *_mutex = new QReadWriteLock();

    // Auxiliary flags
    bool _zero;

    // Filters
    KalmanFilter2D _kalmanFilter = KalmanFilter2D();
    Noise _noiseFilter = Noise();
    Loss _lossFilter = Loss();

    // Brute velocity calc

    //ObjectVelocity _objVel;



public:
    Object(bool enableLossFilter, bool enableKalmanFilter, bool enableNoiseFilter);
    virtual ~Object();
    void update(double confidence, Position pos, Angle ori);

    bool isValid;
    Position _position = Position();
    Velocity _velocity =  Velocity();
    Angle _orientation = Angle();
    double getConfidence();
    Position getPosition();
    Velocity getVelocity();
    Angle getOrientation();
    void setFilterTimes(double time);
    void setKalman(bool enable){enableKalman = enable;}
    void setLoss(bool enable){enableLoss = enable;}
    void setNoise(bool enable){enableNoise = enable;}
    void predict();
    void setUnknown();
    bool checkNoise();
    bool checkLoss();


};

#endif // OBJECT_H
