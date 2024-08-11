#include <boost/asio.hpp>
#include "Boost_Server_Class.hpp"
#include <iostream>
using namespace std;
int main() {
    try {
        boost::asio::io_context io_context;
        ServerClass server(io_context);
        io_context.run();
    }
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    return 0;
}

