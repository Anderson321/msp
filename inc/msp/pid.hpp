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
    pid(double Kp, double Ki, double Kd);

    double compute_output(double input, double desired);
    void set_sampling(unsigned int freq);
    void set_outbounds(double min, double max);
    void set_mode(unsigned int mode);

    double get_kp();
    double get_ki();
    double get_kd();

    void set_kp(double Kp);
    void set_ki(double Ki);
    void set_kd(double Kd);

private:
    /* timing */
    clock_t prev_time;
    unsigned long sample_time;

    /* state variables */
    double prev_input, prev_desired;
    double prev_error;
    double max_out, min_out;

    /* tuning constants */
    double kp, ki, kd;
    double i_term;

    bool auto_control;
};

#endif /* PID_CONTROLLER_ */
