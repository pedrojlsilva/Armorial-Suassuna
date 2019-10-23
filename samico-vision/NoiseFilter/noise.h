#ifndef NOISE_H
#define NOISE_H
#include <time.h>

class Noise{
	public:
        Noise();
		void initCounter();
		void setOff();
		bool isInitialized();
		bool noiseFilter();
		void setFilterTime(double time);
	private:
        double filterTime = 300.0;
		double temporizer;
        bool initialized = false;
};

#endif
