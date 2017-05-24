#include "computerVision"
#include <iostream>
#include <fstream>

namespace cv {

	computerVision::computerVision(pipe *file) {

		int value;

		std::ifstream ifs (pipe, std::ifstream::in);
		char c = ifs.get();
		if (ifs.good()) {
			this->hasShoes << c;
			c = ifs.get();
			this->horizontalDistance << c;
			c = ifs.get();
			this->distance << c;


		}


	}





















}


