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
	this->temporizer.start(filterTime);
	this->initialized = true;
}

bool Noise::noiseFilter(){
    if(!(this->temporizer.isActive())){ // se o temporizador não estiver ativo -> terminou o filtro
		return true;
	}

	return false;
}

/* end of noise filter methods */
