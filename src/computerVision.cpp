#include "computerVision.hpp"

namespace cv {

  /**
   * ======================================================
   * CLASS: computerVision 
   * ======================================================
   **/

  /*
   * Constructs computerVision object for the given named
   * pipe string.
   */
	computerVision::computerVision(char *pipe) {
		this->pipe = pipe;

		readPipe(pipe);
		
	}

	/*
   * Updates the fields by accepting a new pipe name.
   */
	void computerVision::update(char *pipe, prevPoint *prev) {

		int tempHor = this->horizontalPixels;
		int tempVer = this->verticalPixels;

		readPipe(pipe);

		if(this->horizontalPixels != -1 && this->verticalPixels != -1) {
			prev->update(tempHor, tempVer);
		}
		

		

		
	}

	// returns if the line is within vertical and horizontal bounds
	bool computerVision::hasShoes() {
		return((verticalPixels <= V_RANGE && verticalPixels >= 0) &&
				(horizontalPixels <= H_RANGE && horizontalPixels >= 0);
	}

	// returns if the shoelace is centered. Revisit these values
	bool computerVision::isCentered() {
		return (horizontalPixels <= H_CENTERRIGHT && horizontalPixels >= H_CENTERLEFT && 
				verticalPixels <= V_CENTERBOTTOM && verticalPixels >= V_CENTERTOP);
	}

	bool computerVision::isCloseCentered() {
		return (horizontalPixels <= H_CENTERRIGHT && horizontalPixels >= H_CENTERLEFT);
	}

	bool computerVision::isLeft() {
		return (horizontalPixels < H_CENTERLEFT && horizontalPixels != -1);
	}

	bool computerVision::isRight() {
		return (horizontalPixels > H_CENTERRIGHT && horizontalPixels != -1);
	}

	bool computerVision::isTooLow() {
		return (verticalPixels > V_CENTERBOTTOM && verticalPixels != -1);
	}

	bool computerVision::isTooHigh() {
		return (verticalPixels < V_CENTERTOP && verticalPixels != -1);
	}

	bool computerVIsion::inRange() {
		return (this->proxDistance <= INRANGE); 
	}

	// get pixelFactor experimentally
	double computerVision::getDistanceDifference(int code) {
		double difference;
		if (code == 0) {
			difference = V_CENTERTOP - verticalPixels;
		} else if (code == 1) {
			difference = V_CENTERBOTTOM - verticalPixels;
		} else if (code == 2) {
			difference = horizontalPixels - H_CENTERRIGHT;
		} else { 
			difference = horizontalPixels - H_CENTERLEFT;
		}

		return difference * pixelFactor;
	}

	int computerVision::getProxDistance() {
		return this->proxDistance;
	}

	int computerVision::readString(char *pipeString, int index) {
    	std::string s;
    	while(pipestring[index] != " ") {
    		s += pipeString[index];
    		index++;
    	}
		
		return std::stoi(s);		
	}

	void computerVision::readPipe(char *pipe) {

		std::ifstream ifs;
		ifs.open(pipe, std::ifstream::in);

		char pipeString[14]; 
		ifs.get(s, 14);
		

		// Sets the fields by reading the pipe
		this->horizontalPixels = readString(pipeString, 0);

		this->verticalPixels = readString(pipeString, 4);

		this->proxDistance = readString(pipeString, 8);
		
		if(readString(pipeString, 12) == 1) {
			this->irFlag = true;
		} else {
			this->irFlag = false;
		}

	}

	bool computerVision::getIRFlag() {
		return this->irFlag;
	}


	/**
	* ======================================================
	* CLASS: prevPoint 
	* ======================================================
	**/

	/*
	* General constructor for a previously seen point.
	*/
	prevPoint::prevPoint() {
		prevPoint(0, 0);
	}

	/*
	* Constructs Point for previous point shoes were seen at
	* in the camera frame.
	*/
	prevPoint::prevPoint(int x, int y) {
		this->prevHorizontal = x;
		this->prevVertical   = y;
	}

	/*
	* Updates the point (x, y) values of the prevPoint.
	*/
	void prevPoint::update(int x, int y) {
		this->prevHorizontal = x;
		this->prevVertical   = y;
	}

	/*
	* Returns previous horizontal point where shoes were
	* last seen.
	*/
	int prevPoint::getX() {
		return prevHorizontal;
	}

	/*
	* Returns previous vertical point where shoes were
	* last seen.
	*/
	int prevPoint::getY() {
		return prevVertical;
	}


 /**
   * ======================================================
   * CLASS: prevRC
   * ======================================================
   **/

   /* General constructor for previous RC set to FC */
	prevRC::prevRC() {
		prevRC(0,0,0,0,0,0,0,0);	
	}

    prevRC::prevRC(int roll, int pitch, int yaw, int throttle, int aux1, int aux2, int aux3, int aux4) {
    	this->roll = roll;
    	this->pitch = pitch;
    	this->yaw = yaw;
    	this->throttle = throttle;
    	this->aux1 = aux1;
    	this->aux2 = aux2;
    	this->aux3 = aux3;
    	this->aux4 = aux4;

    }
    int prevRC::getRoll() {
    	return roll;
    }
    int prevRC::getPitch() {
    	return pitch;
    }
    int prevRC::getYaw() {
    	return yaw;
    }
    int prevRC::getThrottle() {
    	return throttle;
    }
    int prevRC::getAux1() {
    	return aux1;
    }
    int prevRC::getAux2() {
    	return aux2;
    }
    int prevRC::getAux3() {
    	return aux3;
    }
    int prevRC::getAux4() {
    	return aux4;
    }
    void update(int[] channel, int[] values) {

   		
    	this->roll = roll;
    	this->pitch = pitch;
    	this->yaw = yaw;
    	this->throttle = throttle;
    	this->aux1 = aux1;
    	this->aux2 = aux2;
    	this->aux3 = aux3;
    	this->aux4 = aux4;
    }
}
