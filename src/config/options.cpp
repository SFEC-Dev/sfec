#include "options.h"

std::map<std::string, std::vector<std::string>> config::custom_options{};
std::map<std::string, config::OPTIONS_TYPE> config::options{};
std::map<std::string, std::variant<int, std::string, tui::Color, bool>> config::config;

void config::setup_options() {
    options["selector_color"] = OPTIONS_TYPE::COLOR;
    options["frame_style"] = OPTIONS_TYPE::CUSTOM;
    options["text_size"] = OPTIONS_TYPE::INT;
    options["transperant"] = OPTIONS_TYPE::BOOL;

    custom_options["frame_style"] = {"none", "default", "between"};
}