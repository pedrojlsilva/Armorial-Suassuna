
#include "Loss.hpp"

/* loss filter methods */

void Loss::setFilterTime(double time){
	this->filterTime = time;
}

bool Loss::lossFilter(bool update){
    if(update){
        temporizer = clock();
        return false; 
    }

	bool ret = (((double)(clock() - temporizer)/CLOCKS_PER_SEC) >= filterTime);
	if(ret){
        temporizer = clock();
    }
    return ret;
}

/* end of loss filter methods */