#include <Client.hpp>
#include <iostream>

#include <msp_msg.hpp>

int main(int argc, char *argv[]) {
    const std::string device = (argc>1) ? std::string(argv[1]) : "/dev/ttyUSB0";
    const uint baudrate = (argc>2) ? std::stoul(argv[2]) : 115200;

    msp::async::Client client;

    client.connect(device, baudrate);

//    std::this_thread::sleep_for(std::chrono::seconds(2));

    //client.cycle();

    client.start();

    //...
    client.cycle();
    client.sendData(100);
    //client.cycle();
    //client.sendData(102);
    //    std::this_thread::sleep_for(std::chrono::seconds(1));
    //    client.sendData(102);
    //    client.cycle();
    //    std::this_thread::sleep_for(std::chrono::seconds(1));
    //    client.sendData(102);
    //    client.cycle();
    //    std::this_thread::sleep_for(std::chrono::seconds(1));
    //    client.sendData(102);

    std::this_thread::sleep_for(std::chrono::seconds(1));

    //std::cout << "stopping" << std::endl;

    client.stop();

    std::cout << "DONE" << std::endl;
}
