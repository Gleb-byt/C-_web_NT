#include "json_helpers.hpp"
#include <iostream>

InputRequest::InputRequest(const std::string & user_text, const std::string & reference) : 
    user_text(user_text),
    reference(reference)
  {}

const InputRequest input_parse(const http::request<http::string_body> & req) {
    std::string raw_body = req.body();

    try {

        json data = json::parse(raw_body);

        std::string user_text = data["user_translation"].get<std::string>();
        std::string reference = data["reference"].get<std::string>();

        std::cout << "User translation: " << user_text << '\n';

        return InputRequest(user_text, reference);
    } catch (json::parse_error & e) {
        std::cerr << "user sent not valid json : "  << e.what() << '\n';
        return InputRequest("","");
    } catch (json::type_error & e) {
        std::cerr << "not valid types of json : " << e.what() << '\n';
        return InputRequest("","");
    }
}

EvaluateRequest::EvaluateRequest(const std::string & user_translation, 
    const std::string & reference, const std::string & original) :
        user_translation(user_translation),
        reference(reference),
        original(original) {}

const EvaluateRequest evaluate_parse(const http::request<http::string_body> & req) {
    std::string raw_body = req.body();

    try {

        json data = json::parse(raw_body);

        std::string user_translation = data["user_translation"].get<std::string>();
        std::string reference = data["reference"].get<std::string>();
        std::string original = data["original"].get<std::string>(); 

        return EvaluateRequest{user_translation, reference, original};

    } catch (json::parse_error & e) {
        std::cerr << "evaluation algorithm got not valid json : "  << e.what() << '\n';
        return EvaluateRequest("","", "");
    } catch (json::type_error & e) {
        std::cerr << "not valid types of json were sent to evaluation algorithm : " << e.what() << '\n';
        return EvaluateRequest("","", "");
    } catch (json::exception & e) {
        std::cerr << "JSON error: " << e.what() << '\n';
        return EvaluateRequest("", "", "");
    }
}
