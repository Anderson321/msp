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
    if(fcu.isFirmwareCleanflight()) {
        if(fcu.enableRxMSP()==1) {
            std::cout<<"RX_MSP enabled, restart"<<std::endl;
            feature_changed = true;
            goto start;
        }

        if(feature_changed) {
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

    if(fcu.isArmed()) {
        std::cout<<"armed after: "<<std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count()<<" ms"<<std::endl;
    }


    //begin defouling

    char *pipe = "test";

    cv::computerVision cv(pipe);  /* where is pipe initialized? */

    while(!cv.hasShoes()) {
        /* increase throttle and go up */
        fcu.setRc(1500,1500,1500,1500,1000,1000,1000,1000);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        cv.update(pipe);
    }
    /* back off throttle */
    fcu.setRc(1500,1500,1500,1400,1000,1000,1000,1000);

    while(true) {

        while(cv.hasShoes()) {

            while(cv.isTooHigh()) {
                double difference = cv.getDistanceDifference(0);
                // go down a bit
                cv.update(pipe);

            }

            while(cv.isTooLow()) {
                double difference = cv.getDistanceDifference(1);
                // go up a bit
                cv.update(pipe);

            }


            while(cv.isRight()) {
                double difference = cv.getDistanceDifference(2);
                // roll left
                cv.update(pipe);


            }

            while(cv.isLeft()) {
                double difference = cv.getDistanceDifference(3);
                // roll left
                cv.update(pipe);

            }

            while(cv.isCentered()) {
                // go forward 

                goto finish;


            }
        }

        if(cv.prevPoint.getX()


    }





        }
finish: 

    fcu.disarm_block();

}



    
















































}

