#ifndef NOISE_H
#define NOISE_H

#include "Robot/Robot.hpp"
#include "Ball/Ball.hpp"

class Noise{
public:

    Noise();
    void initCounter();
    void setOff();
    bool isInitialized();
    bool noiseFilter();
    void setFilterTime(double time);
private:
    double filterTime;
    double temporizer;
    bool initialized;
};

#endif
