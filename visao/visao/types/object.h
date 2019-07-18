#ifndef OBJECT_H
#define OBJECT_H


#include <QReadWriteLock>
#include "types/position.h"
#include "types/velocity.h"
#include "types/angle.h"
#include "include/filters.h"


class Object
{
public:
    Object();
    ~Object();
    void update(double confidence, const Position &pos, const Angle &ori=Angle(false, 0));


    Position _position = Position();
    Velocity _velocity =  Velocity();
    Angle _orientation = Angle();
    double getConfidence();
    Position getPosition();
    Velocity getVelocity();
    Angle getOrientation();
    bool enableLoss;
    bool enableNoise;
    bool enableKalman;



    void setUnknown();

private:
    // Confidence info
    double _confidence;

    // Thread-safe mutex
    QReadWriteLock _mutex;

    // Auxiliary flags
    bool _zero;

    // Filters

    Loss _lossFilter = Loss();
    KalmanFilter2D _kalmanFilter = KalmanFilter2D();
    Noise _noiseFilter = Noise();

    // Brute velocity calc

    //ObjectVelocity _objVel;

    // Noise filter

    bool _unsafe;

};

#endif // OBJECT_H
