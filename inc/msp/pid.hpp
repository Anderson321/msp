/*
 * Matthew Valuet
 *
 * pid.hpp: Contains all relevant includes, globals, and
 *          classes for a single software PID controller.
 */

#ifndef PID_CONTROLLER_
#define PID_CONTROLLER_

#include <iostream>
#include <fstream>
#include <ctime>

#define DEFAULT_SAMP_SEC 2
#define DEFAULT_MIN_OUT 0
#define DEFAULT_MAX_OUT 255

enum {
    MANUAL,
    AUTOMATIC
};

class pid {
public:
    pid();
    pid(float Kp, float Ki, float Kd);

    float compute_output(float input, float desired);
    void set_sampling(unsigned int freq);
    void set_outbounds(float min, float max);
    void set_mode(unsigned int mode);

    float get_kp();
    float get_ki();
    float get_kd();

    void set_kp(float Kp);
    void set_ki(float Ki);
    void set_kd(float Kd);

private:
    /* timing */
    clock_t prev_time;
    unsigned long sample_time;

    /* state variables */
    float prev_input, prev_desired;
    float prev_error;
    float max_out, min_out;

    /* tuning constants */
    float kp, ki, kd;
    float i_term;

    bool auto_control;
};

#endif /* PID_CONTROLLER_ */
