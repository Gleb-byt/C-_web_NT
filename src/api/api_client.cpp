#include <iostream>

#include "api_client.hpp"


ApiClient::ApiClient(std::string host, std::string port) : 
    host_(host),
    port_(port)
{}


std::pair<bool,json> ApiClient::send_post_request(
    const std::string & target,
    const json & json_body
) {
    try {

        asio::io_context ioc;
    
        tcp::resolver resolver{ioc};
        beast::tcp_stream stream{ioc};
    
        stream.expires_after(std::chrono::seconds(5));
    
        auto const results = resolver.resolve(host_, port_);
    
        stream.connect(results);
    
        std::string body_str = json_body.dump();
    
        http::request<http::string_body> req{http::verb::get, target, 11};
        req.set(http::field::host, host_);
        req.set(http::field::user_agent, "Cpp-Translation-Game/1.0");
        req.set(http::field::content_type, "application/json");
        req.body() = body_str;
        req.prepare_payload();
    
        http::write(stream, req);
    
        beast::flat_buffer buffer;
    
        http::response<http::dynamic_body> res;
    
        http::read(stream, buffer, res);

        beast::error_code ec;
        stream.socket().shutdown(tcp::socket::shutdown_both, ec);

        if (res.result() != http::status::ok) {
            std::cerr << "API Error: status " << res.result_int() << "\n";
            return {false, json{{"error", "HTTP Status" + std::to_string(res.result_int())}}};


        }

        json response_data = json::parse(res.body());

        return {true, response_data};

    }

    catch (const std::exception & e) {
        std::cerr << "ApiClient network error: " << e.what() << "\n";
        return std::make_pair({false, json{{"error", e.what()}}});
    }

    

}