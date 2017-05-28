/*
 * Matthew Valuet
 *
 * pid.cpp: Defines construction and operation of a single
 *          software PID controller.
 *
 *          Assumptions made:
 *          - PID controller set about single "desired"
 *            point and input changed accordingly
 *                => reduce "derivative kick"
 *          - User is responsible for calling
 *            compute_output on either faster or at the same
 *            sampling frequency as set in set_sampling
 *          - User is responsible for setting reasonable
 *            bounds for controller output 
 *          
 *          In use with the quadcopter capstone, a single
 *          instantiation of a PID object should be used
 *          for each axis being controlled.
 */

#include <pid.hpp>

/*
 * Contruct new PID controller initialized at zero for
 * proportional, integral, and derivative constants.
 */
pid::pid() {
    pid::pid(0, 0, 0);
}

/*
 * Construct new PID controller with Kp, Ki, and Kd for the
 * initial controller constants.
 */
pid::pid(double Kp, double Ki, double Kd) {
    sample_time = 1 / DEFAULT_SAMP_SEC;
    prev_time = 0;

    prev_input = prev_desired = 0;
    prev_error = 0;
    set_outbounds(DEFAULT_MIN_OUT, DEFAULT_MAX_OUT);

    kp = Kp;
    ki = Ki;
    kd = Kd;

    auto_control = AUTOMATIC;
}

/*
 * Given an input value and the desired set point,
 * computes the PID output of the controller and returns it.
 */
double pid::compute_output(double input, double desired) {
    if (!auto_control) {
        return 0;
    }

    /* calculate the new time difference */
    double t_diff;
    clock_t now = clock();
    if (prev_time == 0)
        t_diff = sample_time;
    else
        t_diff = (double)(now - prev_time) / CLOCKS_PER_SEC;

    /* enough time has passed to sample again */
    if (t_diff >= sample_time) {
        double d_desired = desired - prev_desired;
        double d_input = input - prev_input;

        /* new error values */
        double error = desired - input;
        double d_error = (d_desired - d_input) / t_diff;

        /* computing integral term within specified bounds */
        i_term += (ki * error * t_diff);

        /* remember variables */
        prev_time = now;
        prev_error = error;

        double ret = (kp * error) + (i_term) + (kd * d_error);
        /* clamp controller output bounds */
        if (ret > max_out)
            return max_out;
        else if (ret < min_out)
            return min_out;
        else
            return ret;
    }

    return 0;
}

/*
 * Sets the controller's sampling period to the given
 * frequency. Do not call after compute_output.
 */
void pid::set_sampling(unsigned int freq) {
    sample_time = 1 / freq;
}

/*
 * Given a maximum and minimum bound, sets the output
 * bounds of the contoller and integral term to disperse
 * with controller reset windup.
 */
void pid::set_outbounds(double min, double max) {
    if (min < max) {
        min_out = min;
        max_out = max;

        /* clamp integral term to limits */
        if (i_term > max_out)
            i_term = max_out;
        else if (i_term < min_out)
            i_term = min_out;
    } else {
        printf("Invalid bounds given... min = %0.3f, max = %0.3f\n", min, max);
    }
}

/*
 * Allows user to specify whether the PID controller moves
 * the output or not based on the given mode.
 */
void pid::set_mode(unsigned int mode) {
    if (mode != MANUAL || mode != AUTOMATIC) {
        printf("Mode option %d invalid...\n", mode);
        printf("Current mode... ");
        if (auto_control) {
            printf("AUTOMATIC\n");
        } else {
            printf("MANUAL\n");
        }
    } else {
        auto_control = mode;
    }
}

/*
 * Sets proportional constant to given value.
 */
void pid::set_kp(double Kp) {
    kp = Kp;
}

/*
 * Sets integral constant to given value.
 */
void pid::set_ki(double Ki) {
    ki = Ki;
}

/*
 * Sets derivative constant to given value.
 */
void pid::set_kd(double Kd) {
    kd = Kd;
}

/*
 * Returns current proportional constant.
 */
double pid::get_kp() {
    return kp;
}

/*
 * Returns current integral constant.
 */
double pid::get_ki() {
    return ki;
}

/*
 * Returns current derivative constant.
 */
double pid::get_kd() {
    return kd;
}
