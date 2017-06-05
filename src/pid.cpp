/*
 * Matthew Valuet
 *
 * pid.cpp: Contains function definitions and helpers for
 *          the PID class/object.
 */

#include <pid.hpp>

/*
 * Constructs a PID controller given the initial Kp, Ki,
 * and Kd controller constants.
 */
PID::PID(float kp, float ki, float kd) {
  this->kp = kp;
  this->ki = ki;
  this->kd = kd;

  min = 0;
  max = 1000;
  iTerm = 0;
  prevInput = prevOutput = 0;
  prevMode = autoMode = AUTOMATIC;
  prevTime = TimePoint();
}

/*
 * Given an input value and the desired point,
 * computes/runs the input through the PID controller and
 * returns the output result.
 */
float PID::computeOutput(float input, float setpoint) {
  /* checking controller mode */
  if (!autoMode) {
    return 0;
  } else if (prevMode == MANUAL && autoMode == AUTOMATIC) {
    prevInput = input;
    prevMode = autoMode;

    iTerm = prevOutput;
    if (iTerm > max) {
      iTerm = max;
    } else if (iTerm < min) {
      iTerm = min;
    }
  }

  /* time since last sample */
  float timeDiff;
  auto currentTime = HighResClock::now();
  if (operator!=(prevTime, TimePoint())) {
    std::cout << "not equal..." << std::endl;
    timeDiff = std::chrono::duration_cast<std::chrono::duration<float>>
                                      (currentTime - prevTime).count();
  } else {
    std::cout << "equal..." << std::endl;
    auto tempTime = HighResClock::now();
    timeDiff = std::chrono::duration_cast<std::chrono::duration<float>>
                                      (tempTime - currentTime).count();
  }
                                      
  /* calculating new error values */
  float error = setpoint - input;
  float deltaError = (input - prevInput) / timeDiff;

  iTerm += (ki * error) * timeDiff;
  if (iTerm > max) {
    iTerm = max;
  } else if (iTerm < min) {
    iTerm = min;
  }

  /* remember variables for next iteration */
  prevInput = input;
  prevTime = currentTime;
  float output = (kp * error) + (iTerm) - (kd * deltaError);
  if (output > max) {
    output = max;
  } else if (output < min) {
    output = min;
  }
  prevOutput = output;

  return output;
}

/*
 * Given a minimum and maximum bound,
 * sets the max/min bounds and clamps the output & integral
 * term to the new bounds.
 */
void PID::setOutputBounds(float min, float max) {
  if (min < max) {
    this->min = min;
    this->max = max;
  }

  /* clamp iTerm to bounds */
  if (iTerm > this->max) {
    iTerm = this->max;
  } else if (iTerm < this->min) {
    iTerm = this->min;
  }
}

/*
 * Given a valid controller mode,
 * updates the current mode and remember the previous mode.
 * Valid controller modes are MANUAL or AUTOMATIC.
 */
void PID::setMode(int mode) {
  if (mode != MANUAL && mode != AUTOMATIC) {
    std::cout << "invalid mode passed... use either MANUAL or AUTOMATIC..."
              << std::endl;
  } else {
    this->prevMode = !mode;
    this->autoMode = mode;
  }
}

/*
 * Returns the current proportional constant.
 */
float PID::getKp() {
  return this->kp;
}

/*
 * Returns the current integral constant.
 */
float PID::getKi() {
  return this->ki;
}

/*
 * Returns the current derivative constant.
 */
float PID::getKd() {
  return this->kd;
}

/*
 * Given a new proportional constant,
 * sets the current proportional constant to the new one.
 */
void PID::setKp(float kp) {
  this->kp = kp;
}

/*
 * Given a new integral constant,
 * sets the current integral constant to the new one.
 */
void PID::setKi(float ki) {
  this->ki = ki;
}

/*
 * Given a new derivative constant,
 * sets the current derivative constant to the new one.
 */
void PID::setKd(float kd) {
  this->kd = kd;
}
