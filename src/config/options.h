#pragma once 

#include <string>
#include <map>
#include <vector>
#include <variant>

#include "../tui/utils/style.h"

namespace config {
    enum class OPTIONS_TYPE{
        INT,
        BOOL,
        COLOR,
        CUSTOM
    };

    using config_t = std::variant<int, std::string, tui::Color, bool>;

    extern std::map<std::string, std::vector<std::string>> custom_options;
    extern std::map<std::string, OPTIONS_TYPE> options;
    extern std::map<std::string, config_t> config;

    void setup_options();

}