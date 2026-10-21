#pragma once

#include <fstream>
#include <iostream>
#include <random>
#include <string>


constexpr int OBJ_AM = 100;

class TextBank {
public:
    std::string read_line(std::string & file_path, int line);
    int random_validation();

};