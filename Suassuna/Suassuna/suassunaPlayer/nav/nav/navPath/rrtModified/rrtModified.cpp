#include "rrtModified.h"



namespace ArmorialPath{

RRTModified::RRTModified(bool **world, float timeLimit){
    _world = world;
    _timeLimit=timeLimit;
}

RRTModified::~RRTModified(){
    delete _world;
    _timeLimit = 0;
};

void RRTModified::iterate(){

}
QList<QPair<int,int>> RRTModified::getPath(){
    return _path;
}

}

