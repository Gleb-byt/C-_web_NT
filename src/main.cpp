#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <memory>


namespace beast = boost::beast;
namespace http = beast::http;


static constexpr auto port = 3000u;

using boost::asio::ip::tcp;


std::string read_file(const std::string & filepath);
std::string_view mime_type(std::string_view path);
void send_text(tcp::socket & socket, http::status status, 
    std::string_view content_type, std::string body,
    unsigned version);

bool send_file(tcp::socket & socket, const std::string & filepath, unsigned version);

void handle_request(tcp::socket & socket, http::request<http::string_body> & req);

int main() {
    try {
        boost::asio::io_context io_context{};
        tcp::acceptor acceptor(io_context, { tcp::v4(), port });
        
        std::cout << "Server listening on http://localhost:" << port << "...\n";
        
        for (;;) {
            tcp::socket socket{io_context};
            acceptor.accept(socket);
            
            
            
            
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

std::string_view mime_type(std::string_view path) {
    auto pos = path.rfind('.');
    if (pos == std::string::npos) return "application/octet-stream";
    
    auto ext = path.substr(pos);
    
    if (ext == ".html" || ext == ".htm") return "text/html; charset=utf-8";
    if (ext == ".css") return "text/css";
    if (ext == ".js") return "application/javascript";
    if (ext == ".json") return "application/json";
    if (ext == ".png") return "image/png";
    if (ext == ".jpg" || ext == ".jpeg") return "image/jpeg";
    if (ext == ".svg") return "image/svg+xml";
    if (ext == ".ico") return "image/x-icon";
    return "application/octet-stream";
}

void send_text(tcp::socket & socket, http::status status, 
    std::string_view content_type, std::string body,
    unsigned version) {
    
    http::response<http::string_body> res{status, version};
    res.set(http::field::server, "Beast-Server");
    res.set(http::field::content_type, content_type);
    res.body() = std::move(body);
    res.prepare_payload();
    http::write(socket, res);
}

bool send_file(tcp::socket & socket, const std::string & filepath, unsigned version) {
    beast::error_code ec;
    
    http::response<http::file_body> res{http::status::ok, version};
    res.body().open(filepath.c_str(), beast::file_mode::scan, ec);
    
    if (ec) return false;
    
    res.set(http::field::server, "Beast-Server" );
    res.set(http::field::content_type, mime_type(filepath));
    res.prepare_payload();
    http::write(socket, res);
    
    return true;
    
    
}


void handle_request(tcp::socket & socket, http::request<http::string_body> & req) {
    auto ver = req.version();
    auto method = req.method();
    auto target = req.target();

    std::cout << method << " " << target << "\n";

    static const std::string static_root = "src/static";

    if (method == http::verb::get && target == "/") {
        if (!send_file(socket, static_root + "/index.html", ver)) {
            send_text(socket, http::status::not_found, "text/plain", "index.html not found", ver);
        }
        return ;
    }

}
