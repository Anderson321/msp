#ifndef COMPUTERVISION_HPP
#define COMPUTERVISION_HPP

namespace cv {

typedef unsigned int uint;

class computerVision {
public: 

	int horizontalPixels;
	int verticalPixels;
	double proxDistance;
	
	
	// initializes cv
	computerVision(pipe *file);
	//updates cv
	void update(pipe *file);

	bool hasShoes();

	bool isCentered();

	double horizontalDistance();

	





}








}

#endif