/*
 * Matthew Valuet
 *
 * pid.h: Contains all relevant includes, using, and class
 *        declarations for the PID controller.
 */

#ifndef PID_CONTROLLER_H_
#define PID_CONTROLLER_H_

#include <iostream>
#include <chrono>

using HighResClock = std::chrono::high_resolution_clock;
using TimePoint = std::chrono::time_point<HighResClock>;

enum { MANUAL, AUTOMATIC };

class PID {
  public:
    /* Constructor */
    PID(float kp, float ki, float kd);

    /* PID controllers */
    float computeOutput(float input, float setpoint);
    void  setOutputBounds(float min, float max);
    void  setMode(int mode);

    /* Accessors and Setters */
    float getKp();
    float getKi();
    float getKd();
    void  setKp(float kp);
    void  setKi(float ki);
    void  setKd(float kd);

  private:
    bool  autoMode, prevMode;
    float kp, ki, kd;
    float min, max;
    float iTerm;
    float prevInput, prevOutput;
    TimePoint prevTime;
};

#endif  /* PID_CONTROLLER_H_ */
