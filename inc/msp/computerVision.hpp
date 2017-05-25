#ifndef COMPUTERVISION_HPP
#define COMPUTERVISION_HPP

const int centerHorizontal = 25;
const int centerVertical = 120;

namespace cv {

typedef unsigned int uint;

class computerVision {
public: 

	// initializes cv
	computerVision(pipe *file);
	//updates cv
	void update(pipe *file);
	// returns true if 
	bool hasShoes();

	bool isCentered();

	bool isLeft();

	bool isRight();

	bool isTooHigh();

	bool isTooLow();

	double horizontalDistance();




private:
	int horizontalPixels;
	int verticalPixels;
	double proxDistance;
	double pixelFactor;


} ;



}

#endif