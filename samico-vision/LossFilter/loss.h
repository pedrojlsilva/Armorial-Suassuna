#ifndef LOSS_H
#define LOSS_H
#include <time.h>

class Loss{
    public:
        Loss();
        bool lossFilter(bool update);
        void setFilterTime(double time);
	private:
        double filterTime = 300.0;
        double temporizer;
};
#endif
