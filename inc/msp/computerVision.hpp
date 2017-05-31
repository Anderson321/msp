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

/*
 * Holds the frame and control flow logic for detecting
 * the shoelaces.
 */
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
	const char *pipe;
	prevPoint prev;

};

/*
 * Holds value of previous frame point where the shoes
 * were last detected.
 */
class prevPoint {
  public:
    int getX();
    int getY();

  private:
    int prevHorizontal;
    int prevVertical;
};
}

#endif /* COMPUTERVISION_HPP_ */
