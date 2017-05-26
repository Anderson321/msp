#ifndef COMPUTERVISION_HPP_
#define COMPUTERVISION_HPP_


namespace cv {

typedef unsigned int uint;
const int verticalRange = 480;
const int horizontalRange = 640;
const int horizontalCenterLeft = 288;
const int horizontalCenterRight = 352;
const int verticalCenterTop = 96;
const int verticalCenterBottom = 128;

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
	double getDistanceDifference();

private:
	int horizontalPixels;
	int verticalPixels;
	double proxDistance;
	double pixelFactor;

};
}

#endif /* COMPUTERVISION_HPP_ */
