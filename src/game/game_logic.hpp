#pragma once

#include <iostream>
#include <algorithm>
#include <fstream>

class GameRound {
public:

    GameRound(std::string & original_text, std::string & user_answer, float evaluation_user);

private:
    std::string original_text {};
    std::string user_answer {};
    float evaluation_user = 0.0f, evaluation_AI = 0.0f;
    std::string NN_answer {};

};