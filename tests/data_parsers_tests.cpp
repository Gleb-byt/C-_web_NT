#include "..\src\game\text_bank.hpp"
#include <windows.h>

int main() {

    SetConsoleOutputCP(65001);

    TextBank easy_texts("./data/texts_easy.json");

    std::cout << easy_texts.empty() << "\n";

    TextItem item = easy_texts.get_random_text();

    std::cout << item.text << ";" << item.translate << "\n";

    return 0;
}