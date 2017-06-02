#include "pid.h"

/* constructor(s) */
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

float PID::computeOutput(float input, float setpoint) {
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

  auto currentTime = HighResClock::now();
  float timeDiff = std::chrono::duration_cast<std::chrono::duration<float>>
                                          (currentTime - prevTime).count();
  // float timeDiff;
  // if (operator!=(prevTime, TimePoint())) {
  //   std::cout << "not equal..." << std::endl;
  //   timeDiff = std::chrono::duration_cast<std::chrono::duration<float>>
  //                                     (currentTime - prevTime).count();
  // } else {
  //   std::cout << "equal..." << std::endl;
  //   auto tempTime = HighResClock::now();
  //   timeDiff = std::chrono::duration_cast<std::chrono::duration<float>>
  //                                     (tempTime - currentTime).count();
  // }
                                      

  /* 
   * TODO:
   * figure out why timeDiff is freaking out the controller
   */
  float error = setpoint - input;
  float deltaError = (input - prevInput) / timeDiff;

  iTerm += (ki * error) * timeDiff;
  if (iTerm > max) {
    iTerm = max;
  } else if (iTerm < min) {
    iTerm = min;
  }

  prevInput = input;
  prevTime = currentTime;
  float output = (kp * error) + (iTerm) - (kd * deltaError);
  if (output > max) {
    output = max;
  } else if (output < min) {
    output = min;
  }
  prevOutput = output;

  /* diagnostics */
  // std::cout << "[diagnostics]: timeDiff: "   << timeDiff   << std::endl;
  // std::cout << "[diagnostics]: error: "      << error      << std::endl;
  // std::cout << "[diagnostics]: iTerm : "     << iTerm      << std::endl;
  // std::cout << "[diagnostics]: deltaError: " << deltaError << std::endl;
  return output;
}

void PID::setOutputBounds(float min, float max) {
  if (min < max) {
    this->min = min;
    this->max = max;
  }

  if (iTerm > this->max) {
    iTerm = this->max;
  } else if (iTerm < this->min) {
    iTerm = this->min;
  }
}

void PID::setMode(int mode) {
  if (mode != MANUAL && mode != AUTOMATIC) {
    std::cout << "invalid mode passed... use either MANUAL or AUTOMATIC..."
              << std::endl;
  } else {
    this->prevMode = !mode;
    this->autoMode = mode;
  }
}


/* accessors & setters */
float PID::getKp() {
  return this->kp;
}

float PID::getKi() {
  return this->ki;
}

float PID::getKd() {
  return this->kd;
}

void PID::setKp(float kp) {
  this->kp = kp;
}

void PID::setKi(float ki) {
  this->ki = ki;
}

void PID::setKd(float kd) {
  this->kd = kd;
}
