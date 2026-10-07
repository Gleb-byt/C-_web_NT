#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>


namespace beast = boost::beast;
namespace http = beast::http;


static constexpr auto port = 3000u;

using boost::asio::ip::tcp;


std::string read_file(const std::string & filepath);

int main() {
    try {
        boost::asio::io_context io_context{};
        tcp::acceptor acceptor(io_context, { tcp::v4(), port });

        std::cout << "Server listening on http://localhost:" << port << "...\n";

        for (;;) {
            tcp::socket socket{io_context};
            acceptor.accept(socket);


            // boost::asio::streambuf request_buf;
            // boost::system::error_code error;
            // boost::asio::read_until(socket, request_buf, "\r\n\r\n", error);

            // if (error && error != boost::asio::error::eof) {
            //     std::cerr << "Read error: " << error.message() << '\n';
            //     continue;
            // }

            // const std::string body = "<h1>Привет из C++ сервера!</h1><button>Кнопка</button>";
            // std::string response =
            //     "HTTP/1.1 200 OK\r\n"
            //     "Content-Type: text/html; charset=utf-8\r\n"
            //     "Content-Length: " + std::to_string(body.size()) + "\r\n"
            //     "Connection: close\r\n"
            //     "\r\n" +
            //     body;

            
            // boost::asio::write(socket, boost::asio::buffer(response), error);
            
            // socket.shutdown(tcp::socket::shutdown_both, error);


            beast::flat_buffer buffer;
            http::request<http::string_body> req;
            http::read(socket, buffer, req);

            req.method();
            req.target();
            req.body();


            std::string file_path = "test_client/client.html";

            // http::response<http::string_body> res{http::status::ok, req.version()};
            http::response<http::file_body> res{http::status::ok, req.version()};
            res.set(http::field::server, "Boost.Beast Server");
            res.set(http::field::content_type, "text/html; charset=utf-8");
            res.keep_alive(req.keep_alive());
            // res.body() = read_file(file_path);
            beast::error_code ec;

            res.body().open(file_path.c_str(), beast::file_mode::scan, ec);


            if (ec == beast::errc::no_such_file_or_directory) {
                http::response<http::string_body> not_found{http::status::not_found, req.version()};
                not_found.set(http::field::content_type, "text/plain; charset=utf-8");
                not_found.body() = "file not found";
                not_found.prepare_payload();
                http::write(socket, not_found);
            } else {
                res.prepare_payload();
                http::write(socket, res);
            }


            socket.shutdown(tcp::socket::shutdown_send, ec);

        }

    } catch (std::exception & e) {

        std::cout << "Error: " << e.what() << std::endl; 
    }
}


std::string read_file(const std::string & filepath) {
    std::ifstream file(filepath, std::ios::in | std::ios::binary);
    if (!file) {
        return "<h1> 404 file Not found</h1>";
    }

    std::ostringstream ss;
    ss << file.rdbuf();
    return ss.str();
}