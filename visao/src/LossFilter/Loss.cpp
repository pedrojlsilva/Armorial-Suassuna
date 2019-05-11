/* loss filter methods */
#include "Loss.hpp"

bool Loss::lossRobotFilter(Robot &newCoord){
	static double iniTime = clock();
	bool ret = (clock() - iniTime >= filterTime);
	if(ret) newCoord.~Robot();
	return ret;
}

bool Loss::lossBallFilter(Ball &newCoord){
	static double iniTime = clock();
	bool ret = (clock() - iniTime >= filterTime);
	if(ret) newCoord.~Ball();
	return ret;
}
 
/* end of loss filter methods */
