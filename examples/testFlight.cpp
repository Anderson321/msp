#include <FlightController.hpp>
#include <computerVision.hpp>
#include <iostream>
#include <chrono>

/* using declarations */
using HighResClock = std::chrono::high_resolution_clock;
using TimePoint = HighResClock::time_point;

/* function declarations */
void ramp_throttle(fcu::FlightController *fcu, int seconds, int throttle, int increment);
void hold_throttle(fcu::FlightController *fcu, int seconds, int throttle);


int main(int argc, char *argv[]) {
    const std::string device = (argc>1) ? std::string(argv[1]) : "/dev/ttyUSB0";
    const uint baudrate = (argc>2) ? std::stoul(argv[2]) : 115200;

    TimePoint start, end;
    bool feature_changed = false;

start:
    fcu::FlightController fcu(device, baudrate);

    // wait until connection is established
    // get unique box IDs
    start = HighResClock::now();
    fcu.initialise();
    end = HighResClock::now();
    std::cout << "ready after: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count()
              << " ms" << std::endl;

    // on cleanflight, we need to enable the
    // "RX_MSP" feature
    if (fcu.isFirmwareCleanflight()) {
        if (fcu.enableRxMSP() == 1) {
            std::cout << "RX_MSP enabled, restart" << std::endl;
            feature_changed = true;
            goto start;
        }

        if (feature_changed) {
            // if we rebooted after updating the RX_MSP
            // feature, we need to sleep for a while
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

    /* wait */
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    // TEST 0
    hold_throttle(10, 1000);
    hold_throttle(1, 1500);
    hold_throttle(1, 1300);
    ramp_throttle(3, 1300, -50);

    // TEST 1
    // /* slowly increase and then hold */
    // ramp_throttle(5, 1500, 10);
    // hold_throttle(1, 1600);

    // /* slowly decrease */
    // ramp_throttle(10, 1600, -5);
    // hold_throttle(1, 1500);
    fcu.disarm_block();
}

/*
 * Given a number of seconds to run, beginning throttle
 * value, and value to increment throttle by,
 * ramps the throttle value by increment over the time period.
 */
void ramp_throttle(fcu::FlightController *fcu, int seconds, int throttle, int increment) {
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
void hold_throttle(fcu::FlightController *fcu, int seconds, int throttle) {
    ramp_throttle(fcu, seconds, throttle, 0);
}

}  /* end cv namespace */
