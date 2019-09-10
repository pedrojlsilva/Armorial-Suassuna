#ifndef OBJECT_H
#define OBJECT_H


#include <QReadWriteLock>
#include "types/types.h"
#include "include/filters.h"
#include <QReadWriteLock>


class Object
{

private:
    bool enableKalman;
    bool enableLoss;
    bool enableNoise;
    double _confidence;

    // Thread-safe mutex
    QReadWriteLock *_mutex = new QReadWriteLock();

    // Auxiliary flags
    bool _zero;

    // Filters

    Loss _lossFilter = Loss();
    KalmanFilter2D _kalmanFilter = KalmanFilter2D();
    Noise _noiseFilter = Noise();

    // Brute velocity calc

    //ObjectVelocity _objVel;



public:
    Object(bool enableLossFilter, bool enableKalmanFilter, bool enableNoiseFilter);
    virtual ~Object();
    void update(double confidence, Position pos, Angle ori);


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
    void setUnknown();




};

#endif // OBJECT_H
