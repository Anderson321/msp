#ifndef COMPUTERVISION_HPP
#define COMPUTERVISION_HPP

namespace cv {

typedef unsigned int uint;

class computerVision {
public: 


	// initializes cv
	computerVision(pipe *file);
	//updates cv
	void update(pipe *file);

	bool hasShoes(void);

	bool isCentered(void);

	double horizontalDistance(void);

private:
	int horizontalPixels;
	int verticalPixels;
	double proxDistance;





} ;










}

#endif