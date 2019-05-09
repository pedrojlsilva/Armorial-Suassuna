class Filters{
	public:

	void setFilterTime(unsigned int time){
		this->filterTime = time;
	}

    void noiseFilter(Robot &newCoord){
		static double initTime = clock();
		static Robot filtCoord = newCoord;
		
		if(clock() - initTime > this->filterTime){
			initTime = clock();
			filtCoord = newCoord;
		}
	}

	void noiseFilter(Ball &newCoord){	
		static double initTimeB = clock();
		static Ball filtCoordB = newCoord;

		if(clock() - initTimeB > this->filterTime){
			initTimeB = clock();
			filtCoordB = newCoord;
		}
	}

	bool lossFilter(Robot &newCoord){
		static double iniTime = clock();
		bool ret = (clock() - iniTime >= this->filterTime);
		if(ret) newCoord.~Robot();
		return ret;
	}

	bool lossFilter(Ball &newCoord){
		static double iniTime = clock();
		bool ret = (clock() - iniTime >= this->filterTime);
		if(ret) newCoord.~Ball();
		return ret;
	}
	
	private:
		unsigned int filterTime;

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

