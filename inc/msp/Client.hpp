#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <asio.hpp>
#include <thread>

#include <types.hpp>

namespace msp {
namespace async {

enum State {
    READ_PREAMBLE,
    READ_DIRECTION,
    READ_SIZE,
    READ_COMMAND,
    READ_PAYLOAD,
    READ_CRC
};

class Client {
public:
    Client() : port(io), running(false) { }

    void connect(const std::string &device, const uint baudrate=115200);

    void start();

    void stop();

    bool sendData(const uint8_t id, const ByteVector &data = ByteVector(0));

    uint8_t crc(const uint8_t id, const ByteVector &data);

    void cycle();

    void onHeaderStart(const asio::error_code& e, std::size_t size);

    void onData(const asio::error_code& e, std::size_t size);

private:
    asio::io_service io;
    asio::serial_port port;
    std::thread thread;
    bool running;

    State state;

    asio::streambuf buffer;
    std::vector<uint8_t> msg;
};

} // namespace async
} // namespace msp

#endif // CLIENT_HPP
