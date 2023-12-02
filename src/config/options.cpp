#include "options.h"

std::map<std::string, std::vector<std::string>> custom_options{};
std::map<std::string, OPTIONS_TYPE> options{};

void setup_options() {
    options["selector_color"] = OPTIONS_TYPE::COLOR;
    options["frame_style"] = OPTIONS_TYPE::CUSTOM;
    options["text_size"] = OPTIONS_TYPE::INT;
    options["transperant"] = OPTIONS_TYPE::BOOL;

    custom_options["frame_style"] = {"none", "default", "between"};
}
