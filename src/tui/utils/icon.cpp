#include "icon.h"

#include <map>
#include <string>

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
constexpr const char32_t conf = U'\ue615';
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
constexpr const char32_t json = U'\ue60b';
constexpr const char32_t shell = U'\ue795';
constexpr const char32_t text = U'\uf15c';
constexpr const char32_t license = U'\ue60a';
constexpr const char32_t video = U'\uf1c8';
constexpr const char32_t audio = U'\uf1c7';

const std::map<std::string, char32_t> ext_icons{ { "c", c },        { "cpp", cpp },       { "py", python },     { "html", html },
                                                 { "css", css },    { "js", javascript }, { "ts", typescript }, { "go", golang },
                                                 { "cs", c_sharp }, { "fs", f_sharp },    { "exe", bin },       { "out", bin },
                                                 { "png", image },  { "jpeg", image },    { "vim", vim },       { "java", java },
                                                 { "rs", rust },    { "php", php },       { "h", h },           { "md", markdow },
                                                 { "cfg", conf },   { "json", json },     { "conf", conf },     { "sh", shell },
                                                 { "txt", text },   { "mp4", video },     { "mp3", audio } };

const std::map<std::string, char32_t> filename_icons{ { "CMakeLists.txt", cmake },
                                                      { ".gitignore", git },
                                                      { "LICENSE.txt", license } };

const std::map<std::string, char32_t> filedir_icons{ { ".git", git }, { "Downloads", download_dir } };

const std::map<char32_t, tui::Color> ext_colors{
    { c, tui::Color(85, 85, 85) },
    { cpp, tui::Color(243, 75, 125) },
    { bin, tui::Color(252, 73, 73) },
    { c_sharp, tui::Color(83, 43, 212) },
    { python, tui::Color(232, 218, 71) },
    { html, tui::Color(228, 77, 38) },
    { css, tui::Color(66, 165, 245) },
    { f_sharp, tui::Color(81, 154, 186) },
    { javascript, tui::Color(203, 203, 65) },
    { typescript, tui::Color(81, 154, 186) },
    { cmake, tui::Color(39, 172, 229) },
    { golang, tui::Color(81, 154, 186) },
    { vim, tui::Color(1, 152, 51) },
    { rust, tui::Color(211, 196, 171) },
    { react, tui::Color(19, 84, 191) },
    { haskell, tui::Color(160, 116, 196) },
    { git, tui::Color(241, 76, 40) },
    { java, tui::Color(204, 62, 68) },
    { h, tui::Color(104, 102, 251) },
    { image, tui::Color(76, 123, 218) },
    { php, tui::Color(160, 116, 196) },
    { conf, tui::Color(117, 117, 117) },
    { json, tui::Color(203, 203, 85) },
    { markdow, tui::Color(81, 154, 186) },
    { shell, tui::Color(137, 224, 81) },
    { license, tui::Color(216, 216, 52) },
    { video, tui::Color(31, 149, 231) },
    { audio, tui::Color(239, 83, 132) },

};

tui::icons::icon_t tui::icons::get_icon(const std::filesystem::path& name) {
    icon_t result;
    const auto temporary_filename = name.filename().string();

    if (auto icon = filename_icons.find(temporary_filename); icon != filename_icons.cend()) {
        result.first = icon->second;
    } else {
        auto dot_pos = temporary_filename.rfind(".");
        if (dot_pos == std::string::npos)
            result.first = default_file;

        else if (auto icon = ext_icons.find(temporary_filename.substr(dot_pos + 1)); icon != ext_icons.cend())
            result.first = icon->second;
    }

    if (is_directory(name)) {
        if (auto icon = filedir_icons.find(temporary_filename); icon != filedir_icons.cend()) {
            result.first = icon->second;
        } else {
            std::filesystem::directory_iterator it(name, std::filesystem::directory_options::skip_permission_denied);

            if (it == std::filesystem::end(it)) {
                result.first = empty_dir;
                result.second = tui::Color(200, 200, 200);
            } else
                result.first = dir;
        }
    }

    if (result.first == 0) {
        result.first = default_file;
    }

    if (result.second.is_empty) {
        if (auto color = ext_colors.find(result.first); color != ext_colors.cend()) {
            result.second = color->second;
        } else {
            const tui::Color default_color{ 255, 255, 255 };
            result.second = default_color;
        }
    }

    return result;
}
