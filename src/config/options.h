#pragma once 

#include <string>
#include <map>
#include <vector>

enum class OPTIONS_TYPE{
    INT,
    BOOL,
    COLOR,
    CUSTOM
};

extern std::map<std::string, std::vector<std::string>> custom_options;
extern std::map<std::string, OPTIONS_TYPE> options;

void setup_options();

