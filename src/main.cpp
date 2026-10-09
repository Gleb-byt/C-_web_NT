#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <memory>
#include <stdexcept>


namespace beast = boost::beast;
namespace http = beast::http;


static constexpr auto port = 3000u;

using boost::asio::ip::tcp;


std::string read_file(const std::string & filepath);
std::string_view mime_type(std::string_view path);
void send_text(tcp::socket & socket, http::status status, 
    std::string_view content_type, std::string body,
    unsigned version, bool alive);

bool send_file(tcp::socket & socket, const std::string & filepath, unsigned version, bool alive);

void handle_request(tcp::socket & socket, http::request<http::string_body> & req);

int main() {
    try {
        boost::asio::io_context io_context{};
        tcp::acceptor acceptor(io_context, { tcp::v4(), port });
        
        std::cout << "Server listening on http://localhost:" << port << "...\n";
        
        for (;;) {
            

            tcp::socket socket{io_context};
            acceptor.accept(socket);

            try {
                beast::flat_buffer buffer;
                http::request<http::string_body> req;
                http::read(socket, buffer, req);

                handle_request(socket, req);

                beast::error_code ec;

                socket.shutdown(tcp::socket::shutdown_send, ec);
            } catch( std::exception & e) {
                std::cerr << "Request error: " << e.what() << "\n";
            }
            
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
    unsigned version, bool alive) {
    
    http::response<http::string_body> res{status, version};
    res.set(http::field::server, "Beast-Server");
    res.set(http::field::content_type, content_type);
    res.keep_alive(alive);
    res.body() = std::move(body);
    res.prepare_payload();
    http::write(socket, res);
}

bool send_file(tcp::socket & socket, const std::string & filepath, unsigned version, bool alive) {
    beast::error_code ec;
    
    http::response<http::file_body> res{http::status::ok, version};
    res.body().open(filepath.c_str(), beast::file_mode::scan, ec);
    
    if (ec) return false;
    
    res.set(http::field::server, "Beast-Server" );
    res.set(http::field::content_type, mime_type(filepath));
    res.keep_alive(alive);
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
        if (!send_file(socket, static_root + "/index.html", ver, req.keep_alive())) {
            send_text(socket, http::status::not_found, "text/plain", "index.html not found", ver, req.keep_alive());
        }
        return ;
    }

    //Post method to start the round later here will be the link to normal html page
    if (method == http::verb::get && target.starts_with("/static/")) {
        std::string rel(target.substr(8));

        if (rel.find("..") != std::string::npos) {
            send_text(socket, http::status::bad_request, "text/plain", "Bad request", ver, req.keep_alive());
            return;
        }
        if (!send_file(socket, static_root + "/" + rel, ver, req.keep_alive())) {
            send_text(socket, http::status::not_found, "text/plain", "File not found " + rel, ver, req.keep_alive());
        }
        return;
    }


    //Post method to send the translate on evaluation
    //Later i will add logic here
    if (method == http::verb::post && target == "/start") {
        std::cout << " body : " << req.body() << "\n";
        send_text(socket, http::status::ok, "application/json", 
        R"({"original" : "Hello world", "reference" : "Привет мир"})", ver
        , req.keep_alive());
        return;
    }

    if (method == http::verb::post && target == "/evaluate") {
        std::cout << " body: " << req.body() << "\n";
        send_text(socket, http::status::ok, "application/json",
            R"({"score": 0.85, "feedback" : "Nice tranlation"})", ver,
            req.keep_alive()
        );
        return;
    }

    send_text(socket, http::status::not_found, "text/plain", "Not found", ver, req.keep_alive());

}
