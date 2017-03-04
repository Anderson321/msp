#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <boost/asio.hpp>
#include <thread>

namespace msp {
namespace async {

class Client {
public:
    Client(const std::string &device, const uint baudrate=115200);

    void cycle();

    void onHeaderStart(const boost::system::error_code& e, std::size_t size);

private:
    boost::asio::serial_port port;
    boost::asio::io_service io;
    //std::thread thread;

    boost::asio::streambuf buffer;
};

} // namespace async
} // namespace msp

#endif // CLIENT_HPP
