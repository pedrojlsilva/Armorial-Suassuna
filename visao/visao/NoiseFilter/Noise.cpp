/* noise filter methods */

#include "Noise.hpp"

Noise::Noise(){

}

void Noise::setFilterTime(double time){
    this->filterTime = time;
}

void Noise::setOff(){
    this->initialized = false;
}

bool Noise::isInitialized(){
    return this->initialized;
}

void Noise::initCounter(){
    this->temporizer = clock();
    this->initialized = true;
}

bool Noise::noiseFilter(){

    if(((double)(clock() - temporizer)/CLOCKS_PER_SEC) >= filterTime){
        return true;
    }

    return false;
}


/* end of noise filter methods */
