#include <Client.hpp>

namespace msp {
namespace async {

using namespace asio;

Client::Client(const std::string &device, const uint baudrate) : port(io) {
    port.open(device);

    port.set_option(serial_port::baud_rate(baudrate));
    port.set_option(serial_port::parity(serial_port::parity::none));
    port.set_option(serial_port::character_size(serial_port::character_size(8)));
    port.set_option(serial_port::stop_bits(serial_port::stop_bits::one));

    //thread = std::thread( [this]{ while(true) { io.run(); } } );
}

void Client::cycle() {
    while(true) {
        async_read_until(port, buffer, "$M", std::bind(&Client::onHeaderStart, this, std::placeholders::_1, std::placeholders::_2));
        io.run();
    }
}

void Client::onHeaderStart(const asio::error_code& e, std::size_t size) {
    // ignore data
}

} // namespace async
} // namespace msp
