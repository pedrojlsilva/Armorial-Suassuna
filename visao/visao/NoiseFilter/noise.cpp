/* noise filter methods */

#include "noise.h"

void Noise::setFilterTime(double time){
	this->filterTime = time;
}

void Noise::setOff(){
    initialized = false;
}

bool Noise::isInitialized(){
    return initialized;
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
