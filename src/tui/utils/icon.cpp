#include <string>
#include <map>

#include "icon.h"

std::map<std::string, const char16_t*> ext_icons{
    {"c", icon::c},           {"cpp", icon::cpp},   {"py", icon::python},
    {"html", icon::html},     {"css", icon::css},   {"js", icon::javascript},
    {"ts", icon::typescript}, {"go", icon::golang}, {"cs", icon::c_sharp},
    {"fs", icon::f_sharp},    {"exe", icon::bin},   {"out", icon::bin},

};

std::map<std::string, const char16_t*> filename_icons{
    {"CMakeLists.txt", icon::cmake}, {"gitignor", icon::git}, {"git", icon::git}

};

const char16_t* get_icon(std::string name) {
    if (filename_icons.find(name) != filename_icons.cend()) {
        return filename_icons[name];
    }
    auto dot_pos = name.find(".");
    if (dot_pos == std::string::npos)
        return icon::default_file;

    if (ext_icons.find(name.substr(dot_pos + 1)) != ext_icons.cend()) {
        return ext_icons[name.substr(dot_pos + 1)];
    }

    return icon::default_file;
}
