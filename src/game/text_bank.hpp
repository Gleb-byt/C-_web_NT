#pragma once

#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <random>

#include <nlohmann/json.hpp>

constexpr int OBJ_AM = 100;

using json = nlohmann::json;


// struct MyContainer {
//     void advance();
//     const char & get_current();
// };

// struct MyIterator {
//     using difference_type = std::ptrdiff_t;
//     using value_type = char;
//     using pointer = const char *;
//     using reference = const char &;
//     using iterator_category = std::input_iterator_tag;


//     explicit MyIterator(MyContainer * tgt = nullptr) : target(tgt) {}

//     MyIterator & operator++();

//     bool operator!=(const MyIterator & rhs) const;

//     reference operator*() const;

//     MyContainer * target = nullptr;

// };


// MyIterator begin(MyContainer & tgt) {
//     return MyIterator{&tgt};
// }

// MyIterator end(MyContainer & tgt) {
//     return MyIterator{};
// }

// void foo() {
//     MyContainer c;
//     json j = json::parse(begin(c), end(c));
// }

// Container is now need as it is used only to read 
// bytes and process them into other data.


struct TextItem {
    int id{0};
    std::string text;
    std::string translate;
    std::string difficulty;

};


class TextBank {
public:

    explicit TextBank(const std::string & filepath);

    std::string get_random_text();


    size_t size() const { return items_.size(); }

    bool empty() const { return items_.empty(); }

private:
    std::vector<TextItem> items_;

    std::mt19937 rng_;

};