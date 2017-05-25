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

    cv::computerVision cv(pipe);
    while(!cv.hasShoes()) {
        fcu.setRc(1600,1500,1500,1500,1000,1000,1000,1000);
        cv.update(pipe);
    }
    fcu.setRc(1500,1500,1500,1500,1000,1000,1000,1000);

    while(cv.hasShoes()) {

        while(cv.isCentered()) {
            // go forward 



        }

        while(cv.isTooHigh()) {

            // go down a bit
            cv.update(pipe);

        }

        while(cv.isTooLow()) {

            // go up a bit
            cv.update(pipe);

        }

    
        while(cv.isRight()) {
            // roll left
            cv.update(pipe);


        }

        while(cv.isLeft()) {
            // roll left
            cv.update(pipe);


        }












    }

    fcu.disarm_block();

}



    
















































}

