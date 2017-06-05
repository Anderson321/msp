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

    std::this_thread::sleep_for(std::chrono::seconds(5));
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

    // wait
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    int increment = 0;
    for (int i = 0; i < 20; i++) {
        fcu.setRc(1500, 1500, 1500, 1000, 1500, 1500, 1500, 1500);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    // slowly increase
    for (int i = 0; i < 10; i++) {
        fcu.setRc(1500, 1500, 1500, 1500 + increment, 1500, 1500, 1500, 1500);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        increment += 50;
    }

    // slowly decrease
    increment = 0;
    for (int i = 0; i < 10; i++) {
        fcu.setRc(1500, 1500, 1500, 1850 - increment, 1500, 1500, 1500, 1500);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        increment += 50;
    }
    fcu.disarm_block();
} 
