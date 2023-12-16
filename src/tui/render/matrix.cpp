#include "matrix.h"
#include "../tui.h"

#include <iostream>

tui::render::TerminalMatrix::TerminalMatrix(int width, int height, char32_t filler) : width_(width), height_(height), filler_{filler}, 
                                            matrix_(height, std::vector<std::pair<char32_t, std::string>>(width, std::pair<char32_t, std::string>(filler_, ""))) {}
void tui::render::write(TerminalMatrix& matrix, vec2d where, char32_t letter) {
    matrix[where].first = letter;
}

void tui::render::write(TerminalMatrix& matrix, vec2d where, std::pair<char32_t, std::string> content) {
    matrix[where] = content;
}

void tui::render::write(TerminalMatrix& matrix, vec2d start, std::u32string_view text) {
    int shift = 0;
    for (size_t i = 0; i < text.size(); i++) {
        write(matrix, vec2d(start.x + i + shift, start.y), text[i]);

        if (text[i] > 0xFFFF) {
            matrix[vec2d(start.x + i + shift + 1, start.y)].first = U'\0';
            shift++;
        }
    }
}

void tui::render::write(TerminalMatrix& matrix, vec2d start, TerminalMatrix& from) {
    for (std::size_t row = 0; row < from.height(); row++) {
        for (std::size_t col = 0; col < from.width(); col++) {
             write(matrix, vec2d(col + start.x, row + start.y), from[vec2d(col, row)]);
        }
    }
}

void tui::render::write_styled(TerminalMatrix& matrix, vec2d where, char32_t letter, 
                    Color text_col, Color bg_col, text_flags flags) {
                        
    write(matrix, where, letter);

    matrix[where].second = color::get_style(text_col, bg_col, flags);
}
                    
void tui::render::write_styled(TerminalMatrix& matrix, vec2d start, std::u32string_view text, 
                    Color text_col, Color bg_col, text_flags flags) {

    write(matrix, start, text);

    int shift = 0;
    for (std::size_t col = 0; col < text.size(); col++) { 
        matrix[vec2d(start.x + col + shift, start.y)].second = color::get_style(text_col, bg_col, flags);
        if (text[col] > 0xFFFF) {
             matrix[vec2d(start.x + col + shift + 1, start.y)].second = color::get_style(text_col, bg_col, flags);
             shift++;
        }
    }
}

void tui::render::wipe(TerminalMatrix& matrix, vec2d start, vec2d end) {
    for (std::size_t row = start.y; row < end.y; row++) {
        for (std::size_t col = start.x; col < end.x; col++) {
            matrix[vec2d(col, row)].first = matrix.filler();
            if (matrix[vec2d(col, row)].second != "")
                matrix[vec2d(col, row)].second.clear();
        }
    }
}

constexpr const char* end_seq{"\033[0m"};

void tui::render::interpret(TerminalMatrix& matrix) {
    for (std::size_t row = 0; row < matrix.height(); row++) {
        for (std::size_t col = 0; col < matrix.width(); col++) { 
            if (vec2d(matrix.width(), matrix.height()) == vec2d(g_tui->old_matrix.width(), g_tui->old_matrix.height()) && matrix[vec2d(col, row)] == g_tui->old_matrix[vec2d(col, row)]) {
                continue;
            }
            
            std::cout << "\033[" + std::to_string(row+1) + ";" + std::to_string(col+1) + "H";

            if (matrix[vec2d(col, row)].second != "") {
                std::cout << matrix[vec2d(col, row)].second << converter.to_bytes(matrix[vec2d(col, row)].first) << end_seq;
            } 
            else
                std::cout << converter.to_bytes(matrix[vec2d(col, row)].first);
            
        }
        std::cout << '\n';
    }
}