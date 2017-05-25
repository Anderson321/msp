#include "computerVision.hpp"
#include <iostream>
#include <fstream>

namespace cv {

	computerVision::computerVision(const char *pipe) {
		std::ifstream ifs;
		ifs.open(pipe, std::ifstream::in);
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
	void computerVision::update(const char *pipe) {
		std::ifstream ifs;
		ifs.open(pipe, std::ifstream::in);
		char c = ifs.get();
		if (ifs.good()) {  /* this does not work */
			this->horizontalPixels << c;
			c = ifs.get();
			this->verticalPixels << c;
			c = ifs.get();
			this->proxDistance << c;
		} 
	}

	// returns if the line is within vertical and horizontal bounds
	bool computerVision::hasShoes() {
		return((verticalPixels <= 480 && verticalPixels >= 0) &&
				(horizontalPixels <= 640 && horizontalPixels >= 0);
	}

	// returns if the shoelace is centered. Revisit these values
	bool computerVision::isCentered() {
		return (horizontalPixels <= 352 && horizontalPixels >= 288 && 
				verticalPixels <= 128 && verticalPixels >= 96);
	}

	bool computerVision::isLeft() {
		return(horizontalPixels < 288);
	}

	bool computerVision::isRight() {
		return(horizontalPixels > 352;
	}

	bool computerVision::isTooLow() {
		return(verticalPixels < 96;
	}

	bool computerVision::isTooHigh() {
		return(verticalPixels > 128);
	}

	// get pixelFactor experimentally
	double computerVision::horizontalDistance() {
		return (horizontalPixels * proxDistance * pixelFactor);
	}
}
