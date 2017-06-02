#include <FlightController.hpp>
#include <computerVision.hpp>
#include <iostream>
#include <chrono>


int main(int argc, char *argv[]) {
    const std::string device = (argc>1) ? std::string(argv[1]) : "/dev/ttyUSB0";
    const uint baudrate = (argc>2) ? std::stoul(argv[2]) : 115200;

    std::chrono::high_resolution_clock::time_point start, end;
    bool feature_changed = false;
start:
    fcu::FlightController fcu(device, baudrate);

    // wait until connection is established
    // get unique box IDs
    start = std::chrono::high_resolution_clock::now();
    fcu.initialise();
    end = std::chrono::high_resolution_clock::now();
    std::cout<<"ready after: "<<std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count()<<" ms"<<std::endl;

    // on cleanflight, we need to enable the "RX_MSP" feature
    if (fcu.isFirmwareCleanflight()) {
        if (fcu.enableRxMSP()==1) {
            std::cout<<"RX_MSP enabled, restart"<<std::endl;
            feature_changed = true;
            goto start;
        }

        if (feature_changed) {
            // if we rebooted after updating the RX_MSP feature, we need to sleep for a while
            std::this_thread::sleep_for(std::chrono::seconds(5));
        }
    }

    std::cout<<"Armed? "<<fcu.isArmed()<<std::endl;

    // arm the FC
    std::cout<<"Arming..."<<std::endl;
    start = std::chrono::high_resolution_clock::now();
    fcu.arm_block();
    end = std::chrono::high_resolution_clock::now();

    if (fcu.isArmed()) {
        std::cout<<"armed after: "<<std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count()<<" ms"<<std::endl;
    }


    /* BEGIN DEFOULING */ 
    char *pipe = "test";
    cv::computerVision cv(pipe);
    cv::prevPoint prevPoint();

    /* initially looking for shoes */
    while (!cv.hasShoes()) {
        /* increase throttle and go up */
        fcu.setRc(1500,1500,1500,1500,1000,1000,1000,1000);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        cv.update(pipe, prevPoint);
    }
    /* back off throttle and hover */
    fcu.setRc(1500,1500,1500,1400,1000,1000,1000,1000);

    /* main loop 
     * TODO:
     * - use condition rather than just true
     */
   // while (true) {
        /* cv "sees" shoes but not centered */
        while (cv.hasShoes() && !cv.isCentered()) {
        shoeFound:
            while (cv.isTooHigh()) {
                double difference = cv.getDistanceDifference(0);
                // go down a bit
                cv.update(pipe, prevPoint);
            }

            while (cv.isTooLow()) {
                double difference = cv.getDistanceDifference(1);
                // go up a bit
                cv.update(pipe, prevPoint);
            }

            while (cv.isRight()) {
                double difference = cv.getDistanceDifference(2);
                // roll left
                cv.update(pipe, prevPoint);
            }

            while (cv.isLeft()) {
                double difference = cv.getDistanceDifference(3);
                // roll right
                cv.update(pipe, prevPoint);
            }
        }

        /* if cv loses vision of the shoes */
        if(!cv.hasShoes()) {
            while(prevPoint.getX() > 320)) {
                if(prevPoint.getY() > 240) {
                    /* previous point was in bottom right corner, roll right and decrease throttle */
                    fcu.setRC(1600,1500,1500,1400,1000,1000,1000,1000);    
                } else {
                    /* previous point was in top right corner, roll right and increase throttle */
                    fcu.setRC(1600,1500,1500,1700,1000,1000,1000,1000);
                }
                /* update */
                cv.update(pipe, prevPoint);
                if(cv.hasShoes()) {
                    goto shoeFound;
                }

            }
            while(prevPoint.getX() <= 320)) {
                if(prevPoint.getY() > 240)) {
                    /* previous point was in bottom left corner, roll left and decrease throttle */
                    fcu.setRC(1400,1500,1500,1400,1000,1000,1000,1000);
                } else {
                    /* previous point was in top left corner, roll left and increase throttle */
                    fcu.setRC(1400,1500,1500,1700,1000,1000,1000,1000);
                }
                cv.update(pipe, prevPoint);
                if(cv.hasShoes()) {
                    goto shoeFound;
                }

            }

        }


   //   }

        /* drone is now centered on the mark */
        while (cv.getProxDistance() > TOOCLOSE) {
            // move forward
            if (cv.getIRFlag()) {
                // send cut signal
            }
        }
    

finish: 
    fcu.disarm_block();
}



    
















































}

