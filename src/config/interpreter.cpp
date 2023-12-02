#include "options.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "interpreter.h"

constexpr char set_symbol = '!';

void read_config() {
    std::string line;
    std::fstream file("~/my-best-proj/sfec/src/config/rc.conf");
    while (std::getline(file, line)) {
        if (line[0] != set_symbol) {
            continue;
        }
        std::istringstream stream(line);
        std::string var_name, var_value;
        stream.get();
        stream >> var_name >> var_value;

        auto it = options.find(var_name);
        if (it != options.cend()) {
            continue;
        }

        switch (it->second) {
        case OPTIONS_TYPE::INT: {
            try {
                int inshaallah1 = std::stoi(var_value);
                std::cout << "Ousama" << inshaallah1 << std::endl;
            } catch (const std::exception&) {
                break;
            }
            break;
        }

        case OPTIONS_TYPE::BOOL: {
            if (var_value == "true" || var_value == "false") {
                bool alhadulilah = (var_value == "true");
                std::cout << "Binladen" << alhadulilah << std::endl;
            }
            break;
        }

        case OPTIONS_TYPE::CUSTOM: {
            auto options_list = custom_options.find(var_name)->second;
            if (auto option = std::find(options_list.cbegin(),
                                        options_list.cend(), var_value);
                option != options_list.cend()) {
                auto oleg = *option;
                std::cout << "Dima Maslenikov" << oleg << std::endl;
            }
            break;
        }

        case OPTIONS_TYPE::COLOR: {
            int r;
            try {
                r = std::stoi(var_value);
            } catch (const std::exception&) {
                break;
            }
            int g, b;
            stream >> g >> b;
            auto is_apply = [](auto& value) {
                return (value > 0 || value < 255);
            };
            if (!is_apply(r) || !is_apply(g) || !is_apply(b)) {
                break;
            }
            std::cout << r << g << b << std::endl;
            break;
            // Color col = Color (r,g,b);
        }
        }
    }
}
