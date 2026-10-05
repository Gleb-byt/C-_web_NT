#include <boost/asio.hpp>
#include <string>
#include <iostream>

static constexpr auto port = 3000u;

using boost::asio::ip::tcp;

int main() {
    try {
        boost::asio::io_context io_context{};
        tcp::acceptor acceptor(io_context, { tcp::v4(), port });

        std::cout << "Server listening on http://localhost:" << port << "...\n";

        for (;;) {
            tcp::socket socket{io_context};
            acceptor.accept(socket);


            boost::asio::streambuf request_buf;
            boost::system::error_code error;
            boost::asio::read_until(socket, request_buf, "\r\n\r\n", error);

            if (error && error != boost::asio::error::eof) {
                std::cerr << "Read error: " << error.message() << '\n';
                continue;
            }

            const std::string body = "hi from server\n";
            std::string response =
                "HTTP/1.1 200 OK\r\n"
                "Content-Type: text/plain; charset=utf-8\r\n"
                "Content-Length: " + std::to_string(body.size()) + "\r\n"
                "Connection: close\r\n"
                "\r\n" +
                body;

            
            boost::asio::write(socket, boost::asio::buffer(response), error);
            
            socket.shutdown(tcp::socket::shutdown_both, error);

        }

    } catch (std::exception & e) {

        std::cout << "Error: " << e.what() << std::endl; 
    }
}