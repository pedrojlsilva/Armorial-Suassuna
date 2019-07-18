#ifndef loss
#define LOSS_H



class Loss{
public:
    Loss();
    bool lossRobotFilter();
    void setFilterTime(double time);
private:
    double filterTime;
    double temporizer;
};

#endif
