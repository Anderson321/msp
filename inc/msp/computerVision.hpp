#ifndef COMPUTERVISION_HPP
#define COMPUTERVISION_HPP

namespace cv {

typedef unsigned int uint;

class computerVision {
public: 

	bool hasShoes;
	double horizontalDistance;
	double distance;
	
	
	// initializes cv
	computerVision(pipe *file);
	//updates cv
	void update(pipe *file);





}








}

#endif