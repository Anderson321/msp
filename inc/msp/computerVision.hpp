#ifndef COMPUTERVISION_HPP_
#define COMPUTERVISION_HPP_

#include <iostream>
#include <fstream>




/* camera frame constants */
#define V_RANGE 480
#define V_CENTERTOP 96
#define V_CENTERBOTTOM 128
#define H_RANGE 640
#define H_CENTERLEFT 288
#define H_CENTERRIGHT 352
#define INRANGE 25

namespace cv {

/*
 * Holds the frame and control flow logic for detecting
 * the shoelaces.
 */
class computerVision {
  public: 
    /* constructor(s) */
    computerVision(char *pipe);

    /* cv state */
    void update(char *pipe);
    bool hasShoes();
    bool getIRFlag();
    int getProxDistance();
    double getDistanceDifference();
    bool hasCut();

    /* positioning */
    bool isCentered();
    bool isCloseCentered();
    bool isLeft();
    bool isRight();
    bool isTooHigh();
    bool isTooLow();
    bool inRange();

    /* reading the pipe */
    int readString(char *pipeString, int index);
    void readPipe(char *pipe);





  private:
    int horizontalPixels;
    int verticalPixels;
    bool irFlag;
    double proxDistance;
    double pixelFactor;

    // does this need to be a const?
    const char *pipe;
};

/*
 * Holds value of previous frame point where the shoes
 * were last detected.
 */
class prevPoint {
  public:
    prevPoint();
    prevPoint(int x, int y);
    int getX();
    int getY();
    void update(int x, int y);

  private:
    int prevHorizontal;
    int prevVertical;
};

/*
 * Holds value of previous quadcopter RC values. 
 */
class prevRC {
  public:
    /* constructors */
    prevRC();
    prevRC(int roll, int pitch, int yaw, int throttle,
           int aux1, int aux2, int aux3, int aux4);

    /* get values */
    int getRoll();
    int getPitch();
    int getYaw();
    int getThrottle();
    int getAux1();
    int getAux2();
    int getAux3();
    int getAux4();

    /* update the previous RC specified by channel */
    void update(int channels[], int values[]);

  private: 
    int values[8];

};




}

#endif  /* COMPUTERVISION_HPP_ */
