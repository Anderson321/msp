#include "computerVision.hpp"
#include <iostream>
#include <fstream>

namespace cv {

	computerVision::computerVision(pipe *file) {
		std::ifstream ifs (pipe, std::ifstream::in);
		char c = ifs.get();
		if (ifs.good()) {
			this->horizontalPixels << c;
			c = ifs.get();
			this->verticalPixels << c;
			c = ifs.get();
			this->proxDistance << c;

		}


	}

	// updates the fields by accepting a new pipe
	void computerVision::update(pipe *file) {
		std::ifstream ifs (pipe, std::ifstream::in);
		char c = ifs.get();
		if (ifs.good()) {
			this->horizontalPixels << c;
			c = ifs.get();
			this->verticalPixels << c;
			c = ifs.get();
			this->proxDistance << c;

		}
    }


    // returns if the line is within vertical and horizontal bounds
	bool computerVision::hasShoes() {
		return((verticalPixels <= 240 && verticalPixels >= -240) ||
				(horizontalPixels <= 320 && horizontalPixels >= -320);

	}



	// returns if the shoelace is centered. Revisit these values
	bool computerVision::isCentered() {
		return (horizontalPixels <= centerHorizontal && horizontalPixels >= -centerHorizontal && 
				verticalPixels <= 240 && verticalPixels >= centerVertical);


	}

	bool computerVision::isLeft() {
		return(horizontalPixels < -centerHorizontal);
	}

	bool computerVision::isRight() {
		return(horizontalPixels > centerHorizontal);
	}

	bool computerVision::isTooLow() {
		return(verticalPixels <= centerVertical);

	}

	bool computerVision::isTooHigh() {
		return(verticalPixels > 240);

	}




	// get pixelFactor experimentally
	double computerVision::horizontalDistance() {
		return (horizontalPixels * proxDistance * pixelFactor);


	}



}


