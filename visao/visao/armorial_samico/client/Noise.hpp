#pragma once
#include <time.h>

class Noise{
	public:
		void initCounter();
		void setOff();
		bool isInitialized();
		bool noiseRobotFilter(Robot &newCoord);
		void noiseBallFilter(Ball &newCoord);
		void setFilterTime(double time);
	private:
		double filterTime;
		double temporizer;
		bool initialized;
};

/* noise filter methods */

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

bool Noise::noiseRobotFilter(Robot &newCoord){

	if(((double)(clock() - temporizer)/CLOCKS_PER_SEC) >= filterTime){
		return true;
	}

	return false;
}

void Noise::noiseBallFilter(Ball &newCoord){	
	static double initTimeB = clock();
	static Ball filtCoordB = newCoord;

	if(((double)(clock() - initTimeB)/CLOCKS_PER_SEC) >= filterTime){
		initTimeB = clock();
		filtCoordB = newCoord;
	}
}

/* end of noise filter methods */