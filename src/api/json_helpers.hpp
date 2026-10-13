#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <nlohmann/json.hpp>

//namespace is naming space so i can use the word only with namespaces

namespace beast = boost::beast;
namespace http = beast::http;

//json is a class so using should be used with objects

using json = nlohmann::json;

struct InputRequest {
    std::string user_text;
    std::string reference;
    InputRequest(const std::string & user_text, const std::string & reference);
};

struct EvaluateRequest {
    std::string user_translation;
    std::string reference;
    std::string original;

    EvaluateRequest(const std::string & user_translation, const std::string & reference, 
        const std::string & original);
};


const InputRequest input_parse(const http::request<http::string_body> & req);
const EvaluateRequest evaluate_parse(const http::request<http::string_body> & req);