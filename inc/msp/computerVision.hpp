#ifndef COMPUTERVISION_HPP_
#define COMPUTERVISION_HPP_

namespace cv {

typedef unsigned int uint;

class computerVision {
public: 

	// initializes cv
	computerVision(char *pipe);
	//updates cv
	void update(char *pipe);
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
};
}

#endif /* COMPUTERVISION_HPP_ */
