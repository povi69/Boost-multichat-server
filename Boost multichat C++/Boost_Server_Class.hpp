#ifndef BOOST_SERVER_CLASS_HPP
#define BOOST_SERVER_CLASS_HPP

#include <boost/asio.hpp>
#include <array>

class ServerClass {
public:
    ServerClass(boost::asio::io_context& io_context);
    ~ServerClass();
    void startAccept();

private:
    void handleAccept(const boost::system::error_code& error);
    void doRead();
    void doWrite(std::size_t length);
    void broadcast(const std::string& message, std::shared_ptr<boost::asio::ip::tcp::socket> sender_socket);
    void welcomeMessage();

    static constexpr auto PORT = 12345;
    static constexpr std::size_t BUFFER_SIZE = 1024;
    boost::asio::ip::tcp::acceptor acceptor_;
    boost::asio::ip::tcp::socket socket_;
    std::array<char, BUFFER_SIZE> data_;
};

#endif // BOOST_SERVER_CLASS_HPP
