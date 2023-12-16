#include "icon.h"

#include <string>
#include <map>

constexpr const char32_t default_file = U'\ue64e';
constexpr const char32_t dir = U'\uf4d3';
constexpr const char32_t empty_dir = U'\uf413';
constexpr const char32_t download_dir = U'\uf498';
constexpr const char32_t document = U'\uf02d';
constexpr const char32_t image = U'\ue60d';
constexpr const char32_t archive = U'\uf187';
constexpr const char32_t bin = U'\ueae8';
constexpr const char32_t git = U'\ue702';
constexpr const char32_t c = U'\ue61e';
constexpr const char32_t cmake = U'\ue673';
constexpr const char32_t cpp = U'\ue61d';
constexpr const char32_t h = U'\uf0fd';
constexpr const char32_t python = U'\ue73c';
constexpr const char32_t javascript = U'\ue781';
constexpr const char32_t typescript = U'\ue628';
constexpr const char32_t java = U'\ue738';
constexpr const char32_t php = U'\ue73d';
constexpr const char32_t ruby = U'\ue791';
constexpr const char32_t c_sharp = U'\ue648';
constexpr const char32_t f_sharp = U'\ue65a';
constexpr const char32_t html = U'\ue736';
constexpr const char32_t css = U'\uf13c';
constexpr const char32_t react = U'\ue7ba';
constexpr const char32_t markdow = U'\ue609';
constexpr const char32_t golang = U'\ue627';
constexpr const char32_t haskell = U'\ue777';
constexpr const char32_t scala = U'\ue737';
constexpr const char32_t kotlin = U'\ue634';
constexpr const char32_t vim = U'\ue62b';
constexpr const char32_t rust = U'\ue7a8';

std::map<std::string, char32_t> ext_icons {
    {"c", c},           {"cpp", cpp},   {"py", python},
    {"html", html},     {"css", css},   {"js", javascript},
    {"ts", typescript}, {"go", golang}, {"cs", c_sharp},
    {"fs", f_sharp},    {"exe", bin},   {"out", bin},
};

std::map<char32_t, tui::Color> ext_colors {
    {c,   tui::Color(225, 225, 55)},
    {cpp, tui::Color(225, 55, 55)},
    {bin, tui::Color(200, 55, 200)}
};

std::map<std::string, char32_t> filename_icons{
    {"CMakeLists.txt", cmake}, {".gitignore", git}, {".git", git}
};

tui::icons::icon_t tui::icons::get_icon(std::filesystem::directory_entry name) {
    icon_t result;
    const auto temporary_filename = name.path().filename().string();

    if (filename_icons.find(temporary_filename) != filename_icons.cend()) {
        result.first = filename_icons[temporary_filename];
    }
    else {
        auto dot_pos = temporary_filename.find(".");
        if (dot_pos == std::string::npos)
            result.first = default_file;

        else if (ext_icons.find(temporary_filename.substr(dot_pos + 1)) != ext_icons.cend())
            result.first = ext_icons[temporary_filename.substr(dot_pos + 1)];
    }

    if (result.first == 0) {
        result.first = default_file;
    }

    if (auto color = ext_colors.find(result.first); color != ext_colors.cend()) {
        result.second = color->second;
    } else {
        const tui::Color default_color{255, 255, 255};
        result.second = default_color;
    }

    return result;
}
