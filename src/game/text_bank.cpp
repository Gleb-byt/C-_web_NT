#include "text_bank.hpp"

#include <memory>

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
                    item.id = elem["id"].get<int>();
                    item.text = elem["text"].get<std::string>();
                    item.translate = elem["translate"].get<std::string>();
                
                    items_.push_back(std::move(item));
                }
            }

            std::cout << "Text Bank Successfully loaded. Size: " << items_.size() << "\n";


        } catch (const std::exception & e) {
            std::cerr << "TextBank JSON Parse Error : " << e.what() << "\n";
        }


}

TextItem TextBank::get_random_text() {
    if (items_.empty()) {
        return TextItem{0,"Hello World!", "Привет мир"};
    }

    std::uniform_int_distribution<size_t> dist(0, items_.size() - 1);

    size_t random_index = dist(rng_);

    return items_[random_index];
}