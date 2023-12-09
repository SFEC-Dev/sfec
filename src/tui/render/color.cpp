#include "color.h"

const std::string start_seq{"\033["};

std::string tui::render::color::get_style(Color text_col, Color bg_col, text_flags flags) {
    std::string result = start_seq;
    
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

void tui::render::write_styled(TerminalMatrix& matrix, pos where, char letter, 
                    Color text_col, Color bg_col, text_flags flags) {
    write(matrix, where, letter);

    auto is_already = matrix.style().try_emplace(where, std::pair<pos, std::string>(pos(where.x + 1, where.y), 
                                                                   color::get_style(text_col, bg_col, flags)));
}
                    
void tui::render::write_styled(TerminalMatrix& matrix, pos start, std::string text, 
                    Color text_col, Color bg_col, text_flags flags) {
    write(matrix, start, text);                   
    auto is_already = matrix.style().try_emplace(start, std::pair<pos, std::string>(pos(start.x + text.size(), start.y), 
                                           color::get_style(text_col, bg_col, flags)));
}