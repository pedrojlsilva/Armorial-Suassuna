/* noise filter methods */

#include "noise.h"
#include <stdio.h>

Noise::Noise(){
    filterTime = 300.0;
}

void Noise::setFilterTime(double time){
    filterTime = time;
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
    if(((double)(clock() - temporizer)/100.0) >= filterTime){
		return true;
	}

	return false;
}

/* end of noise filter methods */
