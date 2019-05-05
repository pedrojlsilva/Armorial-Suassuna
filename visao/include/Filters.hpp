class Filters{
	public:
    void noiseFilter(Robot &newCoord){
		static double initTime = clock();
		static Robot filtCoord = newCoord;
		
		if(clock() - initTime > 500){
			initTime = clock();
			filtCoord = newCoord;
		}
	}

	void noiseFilter(Ball &newCoord){	
		static double initTimeB = clock();
		static Ball filtCoordB = newCoord;

		if(clock() - initTimeB > 500){
			initTimeB = clock();
			filtCoordB = newCoord;
		}
	}
	
	/* to be tested:
	
	bool lossFilter(void){
		static double iniTime = clock();
		static double filterTime=200;
	
		bool ret = clock()-iniTime>=200;
		if(ret) iniTime=clock();
		
		return ret;
	}
*/
};
