#ifndef OBJECT_H
#define OBJECT_H


#include <QReadWriteLock>
#include "types/position.h"
#include "types/angle.h"
#include "include/filters.h"


class Object
{
public:
    Object(bool enableLoss=true, bool enableKalman=true, bool enableNoise=true);
    ~Object();
    void update(double confidence, const Position &pos, const Angle &ori=Angle(false, 0));


    Position _position();
    //Velocity velocity(); // ainda nao usado
    Angle _orientation();
    double confidence();
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

    Loss _lossFilter;

    KalmanFilter2D _kalmanFilter;

    Noise _noiseFilter;

    // Brute velocity calc

    //ObjectVelocity _objVel;

    // Noise filter

    bool _unsafe;

};

#endif // OBJECT_H
