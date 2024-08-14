#pragma once

#include <boost/asio.hpp>
#include <array>
#include <unordered_map>
#include <memory>

class ServerClass {
public:
    /**
     * @brief Constructs the ServerClass object and initializes the acceptor.
     * @param io_context The io_context used to handle asynchronous operations.
     */
    ServerClass(boost::asio::io_context& io_context);

    /**
     * @brief Destructor for ServerClass. Closes all client connections and the acceptor.
     */
    ~ServerClass();

private:
    /**
     * @brief Starts accepting new client connections asynchronously.
     */
    void startAccept();

    /**
     * @brief Starts reading data from a connected client asynchronously.
     * @param socket A shared pointer to the client's TCP socket.
     */
    void startReading(std::shared_ptr<boost::asio::ip::tcp::socket> socket);

    /**
     * @brief Sends data to the connected client asynchronously.
     * @param socket A shared pointer to the client's TCP socket.
     * @param length The length of the data to be written.
     */
    void doWrite(std::shared_ptr<boost::asio::ip::tcp::socket> socket, std::size_t length);

    /**
     * @brief Broadcasts a message to all connected clients except the sender.
     * @param message The message to be broadcasted.
     * @param sender_socket A shared pointer to the sender's TCP socket.
     */
    void broadcast(const std::string& message, std::shared_ptr<boost::asio::ip::tcp::socket> sender_socket);

    void welcomeMessage();

    static constexpr auto PORT = 12345; 
    static constexpr std::size_t BUFFER_SIZE = 1024;  

    boost::asio::ip::tcp::acceptor acceptor_;  
    std::array<char, BUFFER_SIZE> data_;

    /**
     * @brief A map of client sockets with their corresponding file descriptors as keys.
     */
    std::unordered_map<int, std::shared_ptr<boost::asio::ip::tcp::socket>> clients_;
};
