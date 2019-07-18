
#include "Loss.hpp"

Loss::Loss(){

}

void Loss::setFilterTime(double time){
    this->filterTime = time;
}

bool Loss::lossRobotFilter(Robot &newCoord, bool update){
    if(update){
        temporizer = clock();
        return false;
    }

    bool ret = (((double)(clock() - temporizer)/CLOCKS_PER_SEC) >= filterTime);
    if(ret){
        newCoord.~Robot();
        temporizer = clock();
    }
    return ret;
}

bool Loss::lossBallFilter(Ball &newCoord, bool update){
    if(update){
        temporizer = clock();
        return false;
    }

    bool ret = (((double)(clock() - temporizer)/CLOCKS_PER_SEC) >= filterTime);

    if(ret){
        newCoord.~Ball();
        temporizer = clock();
    }
    return ret;
}

