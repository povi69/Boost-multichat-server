#include "Boost_Server_Class.hpp"
#include <iostream>

ServerClass::ServerClass(boost::asio::io_context& io_context)
    : acceptor_(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), PORT)) {
    welcomeMessage();
    startAccept();
}

ServerClass::~ServerClass()
{
    acceptor_.close();
    for (auto& client : clients_)
    {
        if (client.second->is_open())
        {
            client.second->close();
        }
    }
    clients_.clear();
}

void ServerClass::welcomeMessage() {
    std::cout << "Welcome to the server\n";
    std::cout << "Waiting for connections...\n";
}

void ServerClass::startAccept() {
    auto new_socketptr = std::make_shared<boost::asio::ip::tcp::socket>(acceptor_.get_executor());
    acceptor_.async_accept(*new_socketptr, [this, new_socketptr](const boost::system::error_code& error) {
        if (!error) {
            std::cout << "New connection accepted." << std::endl;
            int socket_fd = new_socketptr->native_handle();
            clients_[socket_fd] = new_socketptr;
            startReading(new_socketptr);
            startAccept();
        }
        else {
            throw std::runtime_error("Error accepting connection: " + error.message());
        }
        });
}

void ServerClass::startReading(std::shared_ptr<boost::asio::ip::tcp::socket> socket) {
    socket->async_read_some(boost::asio::buffer(data_), [this, socket](boost::system::error_code ec, std::size_t length) {
        if (!ec)
        {
            std::string message(data_.data(), length);
            std::cout << "Received: " << message << std::endl;
            broadcast(message, socket);
            doWrite(socket, length);
        }
        else
        {
            if (ec == boost::asio::error::eof || ec == boost::asio::error::connection_reset) {
                std::cout << "Client disconnected." << std::endl;
            }
            else {
                clients_.erase(socket->native_handle()); // Remove client from the map
                throw std::runtime_error("Read error: " + ec.message());
            }
            clients_.erase(socket->native_handle()); // Remove client from the map
        }
        });
}


void ServerClass::doWrite(std::shared_ptr<boost::asio::ip::tcp::socket> socket, std::size_t length) {
    boost::asio::async_write(*socket, boost::asio::buffer(data_, length), [this, socket](boost::system::error_code ec, std::size_t /*length*/) {
        if (!ec)
        {
            std::cout << "Message sent" << std::endl;
            startReading(socket);
        }
        else
        {
            clients_.erase(socket->native_handle()); // Remove client from the map if an error occurs
            throw std::runtime_error("Write error: " + ec.message());
        }
        });
}

void ServerClass::broadcast(const std::string& message, std::shared_ptr<boost::asio::ip::tcp::socket> sender_socket)
{
    auto message_ptr = std::make_shared<std::string>(message); //Shared pointer
    for (const auto& client : clients_) {
        if (client.second != sender_socket) {
            boost::asio::async_write(*client.second, boost::asio::buffer(*message_ptr), [this, client, message_ptr](boost::system::error_code ec, std::size_t /*length*/) {
                if (ec)
                {
                    clients_.erase(client.first); // Remove client from the map if an error occurs
                    throw std::runtime_error("Broadcast write error: " + ec.message());
                }
                });
        }
    }
}
