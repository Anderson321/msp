#include <Client.hpp>

#include <iostream>

namespace msp {
namespace async {

using namespace asio;

void Client::connect(const std::string &device, const uint baudrate) {
    port.open(device);

    port.set_option(serial_port::baud_rate(baudrate));
    port.set_option(serial_port::parity(serial_port::parity::none));
    port.set_option(serial_port::character_size(serial_port::character_size(8)));
    port.set_option(serial_port::stop_bits(serial_port::stop_bits::one));
}

void Client::start() {
    state = READ_PREAMBLE;
    thread = std::thread([this]{
        running = true;
        std::cout << "thread start" << std::endl;
        while(running) {
//            cycle();
//            std::cout << "registered" << std::endl;
            //io.poll_one();
            io.run();
//            std::cout << "handled" << std::endl;
        }

    });
}

void Client::stop() {
    running = false;
    io.stop();
    thread.join();
    std::cout << "thread stopped" << std::endl;
}

bool Client::sendData(const uint8_t id, const ByteVector &data) {
    ByteVector msg;
    msg.reserve(6+data.size());

    msg.push_back('$');
    msg.push_back('M');
    msg.push_back('<');
    msg.push_back(uint8_t(data.size()));                // data size
    msg.push_back(id);                                  // message_id
    msg.insert(msg.end(), data.begin(), data.end());    // data
    msg.push_back( crc(id, data) );                     // crc

    const std::size_t bytes_written = asio::write(port, asio::buffer(msg.data(), msg.size()));

    std::cout << "sent: " << uint(id)<<std::endl;

    return (bytes_written==msg.size());
}

uint8_t Client::crc(const uint8_t id, const ByteVector &data) {
    uint8_t crc = uint8_t(data.size())^id;

    for(uint8_t d : data)
        crc = crc^d;

    return crc;
}

void Client::cycle() {
    switch (state) {
    case READ_PREAMBLE:
        async_read_until(port, buffer, "$M", std::bind(&Client::onHeaderStart, this, std::placeholders::_1, std::placeholders::_2));
        break;
    case READ_DIRECTION:
    case READ_SIZE:
    case READ_COMMAND:
        async_read(port, buffer, std::bind(&Client::onData, this, std::placeholders::_1, std::placeholders::_2));
        break;
    case READ_PAYLOAD:
    case READ_CRC:
        std::cerr<<"not implemented" << std::endl;
        break;
    }
}

void Client::onHeaderStart(const asio::error_code& e, std::size_t size) {
    if(e) {
        std::cerr << "error" << std::endl;
        return;
    }
    // ignore data
    std::cout << "header start " << size << "," << buffer.size() << std::endl;
    //buffer.commit(size);
    buffer.consume(size); // remove header

    std::istream is(&buffer);
    //std::vector<char> data((std::istreambuf_iterator<char>(is)), std::istreambuf_iterator<char>());
    //std::vector<char> data(2);
    //is.read(data.data(),data.size());
    std::string s;
    is >> s;

    //std::string line;
    //std::getline(is, line);
//    std::istream is(&buffer);
//    std::ostream os(&buffer);
//    std::string s;
//    is >> s;
//    std::cout<<s;
//    std::cout<<std::endl;
//    state = READ_DIRECTION;
}

void Client::onData(const asio::error_code& e, std::size_t size) {
    if(e) {
        std::cerr << "error" << std::endl;
        return;
    }
    std::cout << "data " << size << std::endl;
}

} // namespace async
} // namespace msp
