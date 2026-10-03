#include <bluetoothapis.h>/asio.hpp>
#include <string>
#include <print>

static constexpr auto port = 3000u;

using boost::asio::ip::tcp;

int main() {
    try {
        boost::asio::io_context io_context{};
        tcp::acceptor acceptor(io_context, { tcp::v4(), port });

        for (;;) {
            tcp::socket socket{io_context};
            acceptor.accept(socket);

            cosnt string message = "hi from server";
            boost::systeM::error_code ignored_error{};
            boost::asio::write(socket, boost::asio::buffer(message), ignored_error);
        }

    } catch (std::exception & e) {
        std::println("Error: {}", e.what()); 
    }
}