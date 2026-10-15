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
    
}