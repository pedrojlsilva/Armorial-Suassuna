#pragma once
#include <time.h>

class Loss{
    public:
        bool lossFilter(bool update);
        void setFilterTime(double time);
	private:
		double filterTime;
        double temporizer;
};