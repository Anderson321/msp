#include "computerVision.hpp"

namespace cv {

  /**
   * ======================================================
   * CLASS: computerVision 
   * ======================================================
   **/

  /*
   * Constructs computerVision object for the given named
   * pipe string.
   */
	computerVision::computerVision(const char *pipe) {
		this->pipe = pipe;
		// this->prev = cv::prevPoint prevPoint();
		// cv::prevPoint prevPoint();

		std::ifstream ifs;
		ifs.open(pipe, std::ifstream::in);
		char c = ifs.get();
		//if (ifs.good()) {
		this->horizontalPixels << c;
		c = ifs.get();
		this->verticalPixels << c;
		c = ifs.get();
		this->proxDistance << c;
		//}
	}

	/*
   * Updates the fields by accepting a new pipe name.
   */
	void computerVision::update(const char *pipe) {
		this->prevPoint.update(this->horizontalPixels, this->verticalPixels);
		std::ifstream ifs;
		ifs.open(pipe, std::ifstream::in);
		char c = ifs.get();
		//if (ifs.good()) {  /* this does not work */
		this->horizontalPixels << c;
		c = ifs.get();
		this->verticalPixels << c;
		c = ifs.get();
		this->proxDistance << c;
		//} 
	}

	// returns if the line is within vertical and horizontal bounds
	bool computerVision::hasShoes() {
		return((verticalPixels <= V_RANGE && verticalPixels >= 0) &&
				(horizontalPixels <= H_RANGE && horizontalPixels >= 0);
	}

	// returns if the shoelace is centered. Revisit these values
	bool computerVision::isCentered() {
		return (horizontalPixels <= H_CENTERRIGHT && horizontalPixels >= H_CENTERLEFT && 
				verticalPixels <= V_CENTERBOTTOM && verticalPixels >= V_CENTERTOP);
	}

	bool computerVision::isLeft() {
		return (horizontalPixels < H_CENTERLEFT && horizontalPixels != -1);
	}

	bool computerVision::isRight() {
		return (horizontalPixels > H_CENTERRIGHT && horizontalPixels != -1);
	}

	bool computerVision::isTooLow() {
		return (verticalPixels > V_CENTERBOTTOM && verticalPixels != -1);
	}

	bool computerVision::isTooHigh() {
		return (verticalPixels < V_CENTERTOP && verticalPixels != -1);
	}

	// get pixelFactor experimentally
	double computerVision::getDistanceDifference(int code) {
		double difference;
		if (code == 0) {
			difference = V_CENTERTOP - verticalPixels;
		} else if (code == 1) {
			difference = V_CENTERBOTTOM - verticalPixels;
		} else if (code == 2) {
			difference = horizontalPixels - H_CENTERRIGHT;
		} else { 
			difference = horizontalPixels - H_CENTERLEFT;
		}

		return difference * pixelFactor;
	}


  /**
   * ======================================================
   * CLASS: prevPoint 
   * ======================================================
   **/

  /*
   * General constructor for a previously seen point.
   */
  prevPoint::prevPoint() {
    prevPoint(0, 0);
  }

  /*
   * Constructs Point for previous point shoes were seen at
   * in the camera frame.
   */
  prevPoint::prevPoint(int x, int y) {
    this->prevHorizontal = x;
    this->prevVertical   = y;
  }

  /*
   * Updates the point (x, y) values of the prevPoint.
   */
  void prevPoint::update(int x, int y) {
    this->prevHorizontal = x;
    this->prevVertical   = y;
  }

  /*
   * Returns previous horizontal point where shoes were
   * last seen.
   */
  int prevPoint::getX() {
    return prevHorizontal;
  }

  /*
   * Returns previous vertical point where shoes were
   * last seen.
   */
  int prevPoint::getY() {
    return prevVertical;
  }
}
