#ifndef loss
#define LOSS_H

#include "Robot/Robot.hpp"
#include "Ball/Ball.hpp"

class Loss{
    public:
    bool lossRobotFilter(Robot &newCoord);
    bool lossBallFilter(Ball &newCoord);

};

#endif
