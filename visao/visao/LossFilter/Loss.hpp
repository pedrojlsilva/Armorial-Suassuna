#ifndef loss
#define LOSS_H


class Loss{
    public:
    bool lossRobotFilter(Robot &newCoord);
    bool lossBallFilter(Ball &newCoord);

};

#endif