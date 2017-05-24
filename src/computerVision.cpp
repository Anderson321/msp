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
		return(verticalPixels <= 240 && verticalPixels >= -240 &&
				horitzontalPixels <= 320 && horizontalPixels >= -320);

	}






	// returns if the 
	bool computerVision::isCentered() {







	}








	double computerVision::horizontalDistance() {







	}




















}


