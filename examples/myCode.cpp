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
                  << " ms"<<std::endl;
    }

    std::this_thread::sleep_for(std::chrono::seconds(10));


    /* initialize pipe, computerVision, prevPoint and prevRC*/ 
    char *pipe = "test";
    cv::computerVision cv(pipe);
    cv::prevPoint prevPoint();
    cv::prevRC prevRC();

    /* initially looking for shoes */
    while (!cv.hasShoes()) {
        /* increase throttle and go up */
        fcu.setRc(1500, 1500, 1500, 1500, 1000, 1000, 1000, 1000);
        prevRC.update(1500, 1500, 1500, 1500, 1000, 1000, 1000, 1000);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        cv.update(pipe, prevPoint);
    }
    /* back off throttle and hover */
    fcu.setRc(prevRC.getRoll(), prevRC.getPitch(), prevRC.getYaw(),
              prevRC.getThrottle() - 100, prevRC.getAux1(), prevRC.getAux2(),
              prevRC.getAux3(), prevRC.getAux3());

    prevRC.update((int[]){0,0,0,1,0,0,0,0}, (int[]){prevRC.getThrottle() - 100});

    /* main loop 
     * TODO:
     * - use condition rather than just true
     */

    shoeFound:
    while (cv.hasShoes() && !cv.isCentered()) {
        while (cv.isTooHigh()) {
            double difference = cv.getDistanceDifference(0);

            /* Increase throttle a bit using PID
            *  Update the previous RC 
            *  Udate the computer Vision and prevPoint
            */
            cv.update(pipe, prevPoint);
        }

        while (cv.isTooLow()) {
            double difference = cv.getDistanceDifference(1);

            /* Decrease throttle a bit using PID
            *  Update the previous RC 
            *  Udate the computer Vision and prevPoint
            */
            cv.update(pipe, prevPoint);
        }

        while (cv.isRight()) {
            double difference = cv.getDistanceDifference(2);

            /* Roll right a bit using PID
            *  Update the previous RC 
            *  Udate the computer Vision and prevPoint
            */
            cv.update(pipe, prevPoint);
        }

        while (cv.isLeft()) {
            double difference = cv.getDistanceDifference(3);

            /* Roll left a bit using PID
            *  Update the previous RC 
            *  Udate the computer Vision and prevPoint
            */
            cv.update(pipe, prevPoint);
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

            fcu.setRC(roll, prevRC.getPitch(), prevRC.getYaw(), throttle,
                                      prevRC.getAux1(), prevRC.getAux2(),
                                      prevRC.getAux3(), prevRC.getAux4());

            /* update */
            prevRc.update((int[]){1,0,0,1,0,0,0,0}, (int[]){roll, throttle});
            cv.update(pipe, prevPoint);
            if (cv.hasShoes()) {
                goto shoeFound;
            }
        }
    }


    /* Drone is now centered on the mark */
    while (!cv.inRange()) {
        // move forward

        cv.update(pipe, prevPoint);
        if (!cv.isCentered()) {
            goto shoeFound;
        }
    }

    /* Defouler is now in range, meaning there is no longer a y component in CV (y = -1), CV just gives
     * the horizontal placement of the vertical line (shoelaces) 
     */

    // THIS NEEDS TO BE DONE


        

        if (cv.getIRFlag()) {
            // send cut signal
        }
    }
    

finish: 
    fcu.disarm_block();
}



    
















































}

