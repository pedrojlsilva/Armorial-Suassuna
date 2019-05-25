#include "matrix.h"

class KalmanState {

public:

    KalmanState();

    void setPosition(float value);

    void setVelocity(float value);

    void setAcceleration(float value);

    float getPosition();

    float getVelocity();

    float getAcceleration();

    Matrix& matrix();

private:

    Matrix _state;

};