#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <asio.hpp>
#include <thread>

namespace msp {
namespace async {

class Client {
public:
    Client(const std::string &device, const uint baudrate=115200);

    void cycle();

    void onHeaderStart(const asio::error_code& e, std::size_t size);

private:
    asio::serial_port port;
    asio::io_service io;
    //std::thread thread;

    asio::streambuf buffer;
};

} // namespace async
} // namespace msp

#endif // CLIENT_HPP
