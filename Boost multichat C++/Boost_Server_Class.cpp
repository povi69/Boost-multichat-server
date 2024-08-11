#include "Boost_Server_Class.hpp"
#include <iostream>

ServerClass::ServerClass(boost::asio::io_context& io_context)
    : acceptor_(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), PORT)),
    socket_(io_context) {
    welcomeMessage();
    startAccept();
}
ServerClass::~ServerClass()
{

}
void ServerClass::welcomeMessage()
{
    std::cout << "Welcome to the server\n";
    std::cout << "Waiting for connections...\n";
}

void ServerClass::startAccept() {
    acceptor_.async_accept(socket_,[this](const boost::system::error_code& error)
        {
            if (!error)
            {
                handleAccept(error);
            }
            
        }); 
}

void ServerClass::handleAccept(const boost::system::error_code& error) {
    if (!error) {
        std::cout << "Connected to the server" << std::endl;
        doRead();
    }
    else
    {
        throw std::runtime_error("Error connecting to the server");
    }
    startAccept();  // Accept next connection
}

void ServerClass::doRead() {
    socket_.async_read_some(boost::asio::buffer(data_),
        [this](boost::system::error_code ec, std::size_t length) {
            if (!ec) {
                std::cout << "Received: " << std::string(data_.data(), length) << std::endl;
                doWrite(length);
            }
            else {
                throw std::runtime_error("Message did not read...");
            }
        });
}

void ServerClass::doWrite(std::size_t length)
{
    boost::asio::async_write(socket_, boost::asio::buffer(data_, length),
        [this](boost::system::error_code ec, std::size_t /*length*/)
        {
            if (!ec)
            {
                doRead();
            }
            else 
            {
                throw std::runtime_error("Message did not sent...");
            }
        });
}
