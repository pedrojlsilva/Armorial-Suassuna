#ifndef NOISE_H
#define NOISE_H

#include "Robot/Robot.hpp"
#include "Ball/Ball.hpp"

class Noise{
	public:
		void noiseRobotFilter(Robot &newCoord);
		void noiseBallFilter(Ball &newCoord);
};

#endif
