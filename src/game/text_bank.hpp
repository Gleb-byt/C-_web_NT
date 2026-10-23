#pragma once

#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <nlohmann/json.hpp>


constexpr int OBJ_AM = 100;

using json = nlohmann::json;


struct MyContainer {
    void advance();
    const char & get_current();
};

struct MyIterator {
    using difference_type = std::ptrdiff_t;
    using value_type = char;
    using pointer = const char *;
    using reference = const char &;
    using iterator_category = std::input_iterator_tag;


    explicit MyIterator(MyContainer * tgt = nullptr) : target(tgt) {}

    MyIterator & operator++();

    bool operator!=(const MyIterator & rhs) const;

    reference operator*() const;

    MyContainer * target = nullptr;

};


MyIterator begin(MyContainer & tgt) {
    return MyIterator{&tgt};
}

MyIterator end(MyContainer & tgt) {
    return MyIterator{};
}

void foo() {
    MyContainer c;
    json j = json::parse(begin(c), end(c));
}

class TextBank {
public:
    std::string read_line(std::string & file_path, int line);
    int random_validation();

};