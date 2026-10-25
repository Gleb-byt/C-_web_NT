#include "text_bank.hpp"

TextBank::TextBank(const std::string & filepath) :
    rng_(std::random_device{}()) {
        std::ifstream file(filepath);
        if (!file.is_open()) {
            std::cerr << "TextBnk Error: Path Error " << filepath << '\n';
            return ;
        }

        try {
            json j = json::parse(file);

            if (j.is_array()) {
                for (const auto & elem : j) {
                    TextItem item;
                    item.id = elem["text"].get<std::string>();
                    
                }
            }

        }


}