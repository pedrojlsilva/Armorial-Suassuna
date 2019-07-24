
#include "ball.h"

QString Ball::name() {

    return "Ball";

}

Ball::Ball(bool enableLossFilter, bool enableKalmanFilter, bool enableNoiseFilter, bool debugDetection) :  Object(enableLossFilter, enableKalmanFilter, enableNoiseFilter){

    _debugDetection = debugDetection;

}


Position Ball::getBallPosition(){
    if(_debugDetection){
        std::cout << "Ball set at (" << ((_position.getX()>0)?" ":"") << _position.getX() << ", " << ((_position.getY()>0)?" ":"") << _position.getY() << "), velocity=" << _velocity.abs() << ".\n";
    }
    return _position;
}

Velocity Ball::getBallVelocity(){
    if(_debugDetection){
        std::cout << "Ball set at (" << ((_position.getX()>0)?" ":"") << _position.getX() << ", " << ((_position.getY()>0)?" ":"") << _position.getY() << "), velocity=" << _velocity.abs() << ".\n";
    }
    return _velocity;
}
/* end of Ball methods */
