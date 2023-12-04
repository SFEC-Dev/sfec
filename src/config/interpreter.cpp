#include "options.h"
#include "interpreter.h"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <filesystem>

constexpr char set_symbol = '!';

bool is_number(std::string_view str) {
    for (auto letter = str.cbegin(); letter != str.cend(); letter++) {
        if (!isdigit(*letter))
            return false;
    }
    return true;
}

int check_color_part(std::string str) {
    if (str.size() > 3)
        return -1;

    if (!is_number(str))
        return -1;
    
    int number = std::stoi(str);

    if (number >= 0 && number <= 255)
        return number;

    return -1;
}

tui::Color to_color(std::string str1, std::string str2, std::string str3) {
    tui::Color result{};

    int r, g, b;
    r = check_color_part(str1);
    g = check_color_part(str2);
    b = check_color_part(str3);
    if (r != -1 && g != -1 && b != -1)
        return tui::Color(r, g, b);
    else
        return tui::Color();

}

void config::read_config() {
    config::setup_options();

    std::string line;
    std::fstream file("./src/config/rc.conf");
    while (std::getline(file, line)) {
        if (line[0] != set_symbol)
            continue;

        std::istringstream line_splitter(line);
        std::string var_name, var_value;

        line_splitter >> var_name >> var_value;   
        if (var_name.empty() || var_value.empty())
            break;

        // Inject '!' from string
        var_name = var_name.substr(1);

        auto it = options.find(var_name);
        if (it == options.cend())
            continue;

        switch (it->second) {
        case OPTIONS_TYPE::INT: {
            if (is_number(var_value))
                config[var_name] = std::stoi(var_value);

            break;
        }

        case OPTIONS_TYPE::BOOL: {
            if (var_value == "true" || var_value == "false")
                config[var_name] = (var_value == "true");

            break;
        }

        case OPTIONS_TYPE::CUSTOM: {
            auto options_list = custom_options.find(var_name)->second;
            if (auto option = std::find(options_list.cbegin(),
                                        options_list.cend(), var_value);
                option != options_list.cend()) {
                config[var_name] = *option;
            }
            break;
        }

        case OPTIONS_TYPE::COLOR: {
            std::string var_value2, var_value3;
            line_splitter >> var_value2 >> var_value3;

            if (var_value2.empty() || var_value3.empty())
                break;
           
            auto color = to_color(var_value, var_value2, var_value3);
            if (!color.is_empty)
                config[var_name] = color;

            break;
        }
        }
    }
}
