#include <string>
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
	computerVision::computerVision(const char *pipe) {
		this->cvPipe = pipe;
		readPipe(this->cvPipe);
	}

	/*
   * Updates the fields by accepting a new pipe name.
   */
	void computerVision::update(const char *pipe, prevPoint *prev) {
		int tempHor = this->horizontalPixels;
		int tempVer = this->verticalPixels;

		readPipe(pipe);

		if (this->horizontalPixels != -1 && this->verticalPixels != -1) {
			prev->update(tempHor, tempVer);
		}
	}

	/* 
   * Returns true if the line is within vertical and
   * horizontal bounds, false otherwise.
   */
	bool computerVision::hasShoes() {
		return ((verticalPixels <= V_RANGE && verticalPixels >= 0)
            && (horizontalPixels <= H_RANGE && horizontalPixels >= 0));
	}

	/*
   * Returns true if the shoelace is centered within the
   * frame, false otherwise.
   */
	bool computerVision::isCentered() {
		return (horizontalPixels <= H_CENTERRIGHT
            && horizontalPixels >= H_CENTERLEFT
            && verticalPixels <= V_CENTERBOTTOM
            && verticalPixels >= V_CENTERTOP);
	}

  /*
   * Returns true if the shoelaces are centered horizontally
   * within the frame, false otherwise.
   */
	bool computerVision::isCloseCentered() {
		return (horizontalPixels <= H_CENTERRIGHT && horizontalPixels >= H_CENTERLEFT);
	}

  /*
   * Returns true if the shoelaces are too far to the left,
   * false otherwise.
   */
	bool computerVision::isLeft() {
		return (horizontalPixels < H_CENTERLEFT && horizontalPixels != -1);
	}

  /*
   * Returns true if the shoelaces are too far to the right,
   * false otherwise.
   */
	bool computerVision::isRight() {
		return (horizontalPixels > H_CENTERRIGHT && horizontalPixels != -1);
	}

  /*
   * Returns true if the shoelaces are too low in the frame,
   * false otherwise.
   */
	bool computerVision::isTooLow() {
		return (verticalPixels > V_CENTERBOTTOM && verticalPixels != -1);
	}

  /*
   * Returns true if the shoeslaces are too high up in the
   * frame, false otherwise.
   */
	bool computerVision::isTooHigh() {
		return (verticalPixels < V_CENTERTOP && verticalPixels != -1);
	}

  /*
   * Returns true if the shoes are within the specified
   * range of the proximity sensor, false otherwise.
   */
	bool computerVision::inRange() {
		return (this->proxDistance <= INRANGE); 
	}

  /*
   * Given the numeric code for which measurement to take,
   * calculates the pixel difference and returns it.
   */
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

  /*
   * Returns true if the CV no longer sees the shoelaces,
   * false otherwise.
   */
	bool computerVision::hasCut() {
		return (this->verticalPixels == -1 && this->horizontalPixels == -1);
	}

  /*
   * Returns the current proximity sensor reading.
   */
	int computerVision::getProxDistance() {
		return this->proxDistance;
	}

  int computerVision::getHeight() {
    return this->height;
  }

  /*
   * Given a string and index to begin reading from,
   * parses the string until it reaches the deliminating
   * character.
   */
	int computerVision::readString(const char *pipeString, int index) {
    std::string s;
    std::string str("_");
    while (str.compare(&pipeString[index]) != 0) {
    	s += pipeString[index];
    	index++;
    }
		
		return std::stoi(s);		
	}

  /*
   * Given the name of a fifo pipe,
   * reads input into a string, parses the information,
   * and saves it.
   */
	void computerVision::readPipe(const char *pipe) {
    std::string cvStr(this->cvPipe);
    std::string pyStr(this->pyPipe);
    std::string inStr(pipe);

    std::ifstream ifs;
    if (inStr.compare(cvStr) == 0) {
      char pipeString[9]; 
      ifs.open(pipe, std::ifstream::in);
      ifs.get(pipeString, 9);
      readcvPipe(pipeString);

    } else if (inStr.compare(pyStr) == 0) {
      char pipeString[11];
      ifs.open(pipe, std::ifstream::in);
      ifs.get(pipeString, 11);
      readpyPipe(pipeString);
    }
	}

  /*
   * Given the string of data from the pipe,
   * reads the horizontal/vertical pixels in.
   */
  void computerVision::readcvPipe(char *str) {
		// Sets the fields by reading the pipe
		this->horizontalPixels = readString(str, 0);
		this->verticalPixels = readString(str, 4);
  }

  /*
   * Given the string of data from the pipe,
   * reads the height, proximity distance, and IR beam.
   */
  void computerVision::readpyPipe(char *str) {
    this->height = readString(str, 0);

		int tempDistance = readString(str, 4);
		if (tempDistance <= this->proxDistance) {
			this->proxDistance = tempDistance;
		}

		if (readString(str, 8) == 1) {
			this->irFlag = true;
		} else {
			this->irFlag = false;
		}
  }

  /*
   * Returns the current value saved for the irFlag.
   */
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

  /*
   * General constructor for previous RC set to FC.
   */
	prevRC::prevRC() {
		prevRC(0,0,0,0,0,0,0,0);	
	}

  /*
   * Constructs prevRC with the given drone RC values.
   */
  prevRC::prevRC(int roll, int pitch, int yaw, int throttle, int aux1, int aux2, int aux3, int aux4) {
    this->values[0] = roll;
    this->values[1] = pitch;
    this->values[2] = yaw;
    this->values[3] = throttle;
    this->values[4] = aux1;
    this->values[5] = aux2;
    this->values[6] = aux3;
    this->values[7] = aux4;
  }

  /*
   * Returns the current ROLL value.
   */
  int prevRC::getRoll() {
  	return this->values[0];
  }

  /*
   * Returns the current PITCH value.
   */
  int prevRC::getPitch() {
  	return this->values[1];
  }

  /*
   * Returns the current YAW value.
   */
  int prevRC::getYaw() {
  	return this->values[2];
  }

  /*
   * Returns the current THROTTLE value.
   */
  int prevRC::getThrottle() {
  	return this->values[3];
  }

  /*
   * Returns the current AUX1 value.
   */
  int prevRC::getAux1() {
  	return this->values[4];
  }

  /*
   * Returns the current AUX2 value.
   */
  int prevRC::getAux2() {
  	return this->values[5];
  }

  /*
   * Returns the current AUX3 value.
   */
  int prevRC::getAux3() {
  	return this->values[6];
  }

  /*
   * Returns the current AUX4 value.
   */
  int prevRC::getAux4() {
  	return this->values[7];
  }

  /*
   * Given the array of channels to change and their values,
   * resets the values of the RC channels.
   */
  void prevRC::update(int channels[], int values[]) {
  	int valuesIndex = 0;
  	for (int i = 0; i < 8; i++) {
  		if (channels[i] == 1) {
  			this->values[i] = values[i];
  			valuesIndex++;
  		}
  	}
  }
}
