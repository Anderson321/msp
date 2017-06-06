#include <FlightController.hpp>
#include <computerVision.hpp>
#include <iostream>
#include <chrono>

using HighResClock = std::chrono::high_resolution_clock;
using TimePoint = HighResClock::time_point;

int main(int argc, char *argv[]) {
    const std::string device = (argc>1) ? std::string(argv[1]) : "/dev/ttyUSB0";
    const uint baudrate = (argc>2) ? std::stoul(argv[2]) : 115200;

    TimePoint start, end;
    bool feature_changed = false;
start:

    /* Put in some sort of loop to make sure that it makes a connection? */
    fcu::FlightController fcu(device, baudrate);

    // wait until connection is established
    // get unique box IDs
    start = HighResClock::now();
    fcu.initialise();
    end = HighResClock::now();
    std::cout << "ready after: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count()
              << " ms" << std::endl;

    // on cleanflight, we need to enable the "RX_MSP" feature
    if (fcu.isFirmwareCleanflight()) {
        if (fcu.enableRxMSP() == 1) {
            std::cout << "RX_MSP enabled, restart" << std::endl;
            feature_changed = true;
            goto start;
        }

        if (feature_changed) {
            // if we rebooted after updating the RX_MSP feature, we need to sleep for a while
            std::this_thread::sleep_for(std::chrono::seconds(5));
        }
    }

    std::cout << "Armed? " << fcu.isArmed() << std::endl;

    // arm the FC
    std::cout << "Arming..." << std::endl;
    start = HighResClock::now();
    fcu.arm_block();
    end = HighResClock::now();

    if (fcu.isArmed()) {
        std::cout << "armed after: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count()
                  << " ms" << std::endl;
    }

    std::this_thread::sleep_for(std::chrono::seconds(10));


    /* initialize pipe, computerVision, prevPoint and prevRC */ 
    const char *pipe = "test";
    cv::computerVision cv(pipe);
    cv::prevPoint prevPoint;
    cv::prevRC prevRC;

    int position[8] = {1, 1, 1, 1, 1, 1, 1, 1};
    int rc_vals[8] = {1500, 1500, 1500, 1800, 1000, 1000, 1000, 1000};
    /* initially looking for shoes */
    while (!cv.hasShoes()) {
        /* increase throttle and go up */
        fcu.setRc(rc_vals[0], rc_vals[1], rc_vals[2], rc_vals[3],
                  rc_vals[4], rc_vals[5], rc_vals[6], rc_vals[7]);
        prevRC.update(position, rc_vals);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        cv.update(pipe, &prevPoint);
        if(failsafe(cv, prevRC)) {
            finish(fcu);
        }
    }
    /* back off throttle and hover */
    fcu.setRc(prevRC.getRoll(), prevRC.getPitch(), prevRC.getYaw(),
              prevRC.getThrottle() - 100, prevRC.getAux1(), prevRC.getAux2(),
              prevRC.getAux3(), prevRC.getAux3());

    for (int i = 0; i < 8; i++) {
        if (i != 3)
          position[i] = 0;
    }
    int throttle_change[1] = {prevRC.getThrottle() - 100};
    prevRC.update(position, throttle_change);

    /* main loop 
     * TODO:
     * - use condition rather than just true
     */
    shoeFound:
    while (cv.hasShoes() && !cv.isCentered()) {
        while (cv.isTooHigh()) {
            double difference = cv.getDistanceDifference(0);

            /* Increase throttle a bit using PID
             * Update the previous RC 
             * Update the computer Vision and prevPoint
             */
            cv.update(pipe, &prevPoint);

            if(failsafe(cv, prevRC)) {
                finish(fcu);
            }
        }

        while (cv.isTooLow()) {
            double difference = cv.getDistanceDifference(1);

            /* Decrease throttle a bit using PID
             * Update the previous RC 
             * Update the computer Vision and prevPoint
             */
            cv.update(pipe, &prevPoint);
            if(failsafe(cv, prevRC)) {
                finish(fcu);
            }
        }

        while (cv.isRight()) {
            double difference = cv.getDistanceDifference(2);

            /* Roll right a bit using PID
             * Update the previous RC 
             * Update the computer Vision and prevPoint
             */
            cv.update(pipe, &prevPoint);
            if(failsafe(cv, prevRC)) {
                finish(fcu);
            }
        }

        while (cv.isLeft()) {
            double difference = cv.getDistanceDifference(3);

            /* Roll left a bit using PID
            *  Update the previous RC 
            *  Udate the computer Vision and prevPoint
            */
            cv.update(pipe, &prevPoint);
            if(failsafe(cv, prevRC)) {
                finish(fcu);
            }
        }
    }

    /* If cv loses vision of the shoes */
    if (!cv.hasShoes()) {
        int roll;
        int throttle;
        while (true) {
            if (prevPoint.getX() > H_RANGE / 2) {
                roll = prevRC.getRoll() + 100;
            } else {
                roll = prevRC.getRoll() - 100;
            }
            
            if (prevPoint.getY() > V_RANGE / 2) {
                throttle = prevRC.getThrottle() - 100;
            } else {
                throttle = prevRC.getThrottle() + 100;
            }

            fcu.setRc(roll, prevRC.getPitch(), prevRC.getYaw(), throttle,
                                      prevRC.getAux1(), prevRC.getAux2(),
                                      prevRC.getAux3(), prevRC.getAux4());

            /* update */
            for (int i = 0; i < 8; i++) {
                if (i != 0 || i != 3)
                  position[i] = 0;
            }
            int updateRollThrottle[2] = {roll, throttle};
            prevRC.update(position, updateRollThrottle);

            cv.update(pipe, &prevPoint);
            if(failsafe(cv, prevRC)) {
                finish(fcu);
            }
            if (cv.hasShoes()) {
                goto shoeFound;
            }
        }
    }


    /* Drone is now centered on the mark */
    while (!cv.inRange()) {
        // move forward

        cv.update(pipe, &prevPoint);
        if(failsafe(cv, prevRC)) {
            finish(fcu);
        }
        if (!cv.isCentered()) {
            goto shoeFound;
        }
    }

    /* Defouler is now in range, meaning there is no longer a y component in CV (y = -1), CV just gives
     * the horizontal placement of the vertical line (shoelaces) 
     */


    while(true){
        while (cv.isCloseCentered()) {
            /* move forward */

            cv.update(pipe, &prevPoint);
            if(failsafe(cv, prevRC)) {
                finish(fcu);
            }
                while (cv.getIRFlag()) {
                    /* move forward very slowly */ 
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                    cv.update(pipe, &prevPoint);
                    if(failsafe(cv, prevRC)) {
                        finish(fcu);
                    }
                    if (cv.hasCut()) {
                        fcu.setRc(prevRC.getRoll(), prevRC.getPitch(), prevRC.getYaw(), prevRC.getThrottle() - 100,
                                          prevRC.getAux1(), prevRC.getAux2(),
                                          prevRC.getAux3(), prevRC.getAux4());

                        std::this_thread::sleep_for(std::chrono::milliseconds(500));

                        fcu.setRc(prevRC.getRoll(), prevRC.getPitch(), prevRC.getYaw(), prevRC.getThrottle() - 200,
                                          prevRC.getAux1(), prevRC.getAux2(),
                                          prevRC.getAux3(), prevRC.getAux4());

                        std::this_thread::sleep_for(std::chrono::milliseconds(500));

                        fcu.setRc(prevRC.getRoll(), prevRC.getPitch(), prevRC.getYaw(), prevRC.getThrottle() - 300,
                                          prevRC.getAux1(), prevRC.getAux2(),
                                          prevRC.getAux3(), prevRC.getAux4());

                        std::this_thread::sleep_for(std::chrono::milliseconds(500));

                        fcu.setRc(1500, 1500, 1500, 1000,
                                          prevRC.getAux1(), prevRC.getAux2(),
                                          prevRC.getAux3(), prevRC.getAux4());

                         std::this_thread::sleep_for(std::chrono::milliseconds(1000));

                         finish(fcu);
                    }
              
            }
        }


        while (cv.isRight()) {
            double difference = cv.getDistanceDifference(2);

            /* Roll right a bit using PID
            *  Update the previous RC 
            *  Udate the computer Vision and prevPoint
            */
            cv.update(pipe, &prevPoint);
            if(failsafe(cv, prevRC)) {
                finish(fcu);
            }


        }

        while (cv.isLeft()) {
            double difference = cv.getDistanceDifference(3);
            
            /* Roll left a bit using PID
             * Update the previous RC 
             * Update the computer Vision and prevPoint
             */
            cv.update(pipe, &prevPoint);
            if(failsafe(cv, prevRC)) {
                finish(fcu);
            }
        }

    }   


}

bool failsafe(computerVision cv, prevRC prevRC) {
    if(cv.getHeight() >= 250) {
        ramp_throttle(10, prevRC.getThrottle(), -25);
    }
}


/*
 * Given a number of seconds to run, beginning throttle
 * value, and value to increment throttle by,
 * ramps the throttle value by increment over the time period.
 */
void ramp_throttle(int seconds, int throttle, int increment) {
    for (int i = 0; i < (seconds * 2); i++) {
        fcu.setRc(1500, 1500, 1500, (throttle + (i * increment)),
                  1500, 1500, 1500, 1500);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

/*
 * Given a number of second to run and start throttle values,
 * holds the throttle value.
 */
void hold_throttle(int seconds, int throttle) {
    ramp_throttle(seconds, throttle, 0);
}


void finish(FlightController fcu) {
    fcu.disarm_block();
}















































