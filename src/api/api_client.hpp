#pragma once

#include <string>
#include <utility>

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <nlohmann/json.hpp>

namespace beast = boost::beast;
namespace asio = boost::asio;
namespace http = beast::http;
using tcp = asio::ip::tcp;
using json = nlohmann::json;

struct TranslationResult {
    bool success{false};
    std::string original_text;
    std::string translation_text;
    std::string error_message;

};

struct EvaluationResult {
    bool success{false};
    double score {0.0};
    std::string feedback;
    std::string error_message;

};

class ApiClient {
public:
    ApiClient(std::string host, std::string port);

    TranslationResult translate(
        const std::string & text
    );

    EvaluationResult evaluate(
        const std::string & user_translation,
        const std::string & reference
    );

private:

    std::string host_;
    std::string port_;

    std::pair<bool, json> send_post_request(
        const std::string & target,
        const json & json_body
    );
};