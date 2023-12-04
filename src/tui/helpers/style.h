#pragma once
#include <algorithm>
#include <string>

namespace tui {
    struct Color {
        Color(int rt, int gt, int bt) : r{std::clamp(rt, 0, 255)}, 
                                        g{std::clamp(gt, 0, 255)}, 
                                        b{std::clamp(bt, 0, 255)} {};

        Color(){ is_empty = true; };
        int r;
        int g;
        int b;

        bool is_empty = false;
    };

    enum FLAGS {
        FLAG_BOLD        = 1 << 0,
        FLAG_ITALIC      = 1 << 1,
        FLAG_DIM         = 1 << 2,
        FLAG_UNDERLINED  = 1 << 3,
        FLAG_BLINK       = 1 << 4,
        FLAG_REVERSE     = 1 << 5,
        FLAG_HIDDEN      = 1 << 6         
    };

    const std::string start_seq{"\033["};
    const std::string end_seq{"\033[0m"};

    typedef int text_flags;
    [[nodiscard]] std::string stylize(std::string_view str, text_flags flags);
    [[nodiscard]] std::string colorize(std::string_view str, Color text_col = Color(), Color bg_col = Color());

}