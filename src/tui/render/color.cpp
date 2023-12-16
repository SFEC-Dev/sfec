#include "color.h"

constexpr const char* start_seq{"\033["};

std::string tui::render::color::get_style(Color text_col, Color bg_col, text_flags flags) {
    std::string result;
    
    if (flags != FLAG_NONE)
        result = start_seq;
    
    if (flags & FLAG_BOLD) {
        result.append("1;");
    }
    if (flags & FLAG_ITALIC) {
        result.append("3;");
    }
    if (flags & FLAG_DIM) {
        result.append("2;");
    }
    if (flags & FLAG_UNDERLINED) {
        result.append("4;");
    }
    if (flags & FLAG_BLINK) {
        result.append("5;");
    }
    if (flags & FLAG_REVERSE) {
        result.append("7;");
    }
    if (flags & FLAG_HIDDEN) {
        result.append("8;");
    }

    if (flags != FLAG_NONE) {
        result.pop_back();
        result.push_back('m');
    }

    if (!text_col.is_empty) {
        result.append("\033[38;2;" + std::to_string(text_col.r) + ";" + 
                                        std::to_string(text_col.g) + ";" + 
                                        std::to_string(text_col.b) + "m");
    }
    if (!bg_col.is_empty) {
        result.append("\033[48;2;" + std::to_string(bg_col.r) + ";" + 
                                        std::to_string(bg_col.g) + ";" + 
                                        std::to_string(bg_col.b) + "m");
    }

    return result;
}