#pragma once

#include <boost/asio.hpp>
#include <array>
#include <unordered_map>
#include <memory>

class ServerClass {
public:
    ServerClass(boost::asio::io_context& io_context);
    ~ServerClass();

private:
    void startAccept();
    void startReading(std::shared_ptr<boost::asio::ip::tcp::socket> socket);
    void doWrite(std::shared_ptr<boost::asio::ip::tcp::socket> socket, std::size_t length);
    void broadcast(const std::string& message, std::shared_ptr<boost::asio::ip::tcp::socket> sender_socket);
    void welcomeMessage();

    static constexpr auto PORT = 12345;
    static constexpr std::size_t BUFFER_SIZE = 1024;
    boost::asio::ip::tcp::acceptor acceptor_;
    std::array<char, BUFFER_SIZE> data_;
    std::unordered_map<int, std::shared_ptr<boost::asio::ip::tcp::socket>> clients_; 
};


