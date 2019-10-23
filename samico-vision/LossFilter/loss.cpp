
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
        this->temporizer->start(filterTime);
        return false; 
    }

    bool ret = !(this->temporizer->isActive()); // se o temporizador não estiver ativo -> deu perda
	
    if(ret){
        this->temporizer->start(filterTime);
    }

    return ret;
}

/* end of loss filter methods */
