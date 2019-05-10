/* noise filter methods */

void Noise::noiseRobotFilter(Robot &newCoord){
	static double initTime = clock();
	static Robot filtCoord = newCoord;
	
	if(clock() - initTime > filterTime){
		initTime = clock();
		filtCoord = newCoord;
	}
}

void Noise::noiseBallFilter(Ball &newCoord){	
	static double initTimeB = clock();
	static Ball filtCoordB = newCoord;

	if(clock() - initTimeB > filterTime){
		initTimeB = clock();
		filtCoordB = newCoord;
	}
}

/* end of noise filter methods */