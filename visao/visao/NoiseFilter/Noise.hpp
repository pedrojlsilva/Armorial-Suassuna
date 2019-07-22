#pragma once
#include <time.h>

class Noise{
	public:
		void initCounter();
		void setOff();
		bool isInitialized();
		bool noiseFilter();
		void setFilterTime(double time);
	private:
		double filterTime;
		double temporizer;
		bool initialized;
};