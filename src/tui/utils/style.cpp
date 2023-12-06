#include "style.h"

std::string tui::stylize(std::string_view str, text_flags flags) {
    std::string result{};

    std::string new_flags{};
;

    if (flags & FLAG_BOLD) {
        new_flags.append("1;");
    }
    if (flags & FLAG_ITALIC) {
        new_flags.append("3;");
    }
    if (flags & FLAG_DIM) {
        new_flags.append("2;");
    }
    if (flags & FLAG_UNDERLINED) {
        new_flags.append("4;");
    }
    if (flags & FLAG_BLINK) {
        new_flags.append("5;");
    }
    if (flags & FLAG_REVERSE) {
        new_flags.append("7;");
    }
    if (flags & FLAG_HIDDEN) {
        new_flags.append("8;");
    }

    if (auto it = str.find(start_seq); it == std::string::npos) {
        new_flags.pop_back();
        result = start_seq + new_flags + 'm' + str.data() + end_seq;

    } else {
        result.append(str);
        result.insert(it, new_flags);
    }

    return result;
}

std::string tui::colorize(std::string_view str, Color text_col, Color bg_col) {
    if (text_col.is_empty && bg_col.is_empty)
        return std::string{str};


    std::string new_flags{};

    if (!text_col.is_empty) {
        new_flags.append("\033[38;2;" + std::to_string(text_col.r) + ";" + 
                                        std::to_string(text_col.g) + ";" + 
                                        std::to_string(text_col.b) + "m");
    }
    if (!bg_col.is_empty) {
        new_flags.append("\033[48;2;" + std::to_string(bg_col.r) + ";" + 
                                        std::to_string(bg_col.g) + ";" + 
                                        std::to_string(bg_col.b) + "m");
    }

    return (new_flags + str.data() + (str.find(end_seq) == std::string::npos ? end_seq : ""));
}
