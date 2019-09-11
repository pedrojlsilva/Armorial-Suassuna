
#include "loss.h"

/* loss filter methods */

Loss::Loss(){
    filterTime = 300.0;
}

void Loss::setFilterTime(double time){
    filterTime = time;
}

bool Loss::lossFilter(bool update){
    if(update){
        temporizer = clock();
        return false; 
    }

    bool ret = (((double)(clock() - temporizer)/100.0) >= filterTime);
	if(ret){
        temporizer = clock();
    }
    return ret;
}

/* end of loss filter methods */
