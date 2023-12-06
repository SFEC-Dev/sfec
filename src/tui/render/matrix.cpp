
#include "matrix.h"

tui::render::pos tui::render::operator+(tui::render::pos lhs, int rhs) {
    lhs.x += rhs;
    lhs.y += rhs;
    return lhs;
}

bool tui::render::operator<(pos lhs, pos rhs) {
    return std::tie(lhs.x, lhs.y) < std::tie(rhs.x, rhs.y);
}

tui::render::TerminalMatrix::TerminalMatrix(int width, int height) : width_(width), height_(height) {
    for (std::size_t row = 0; row < height; row++) {
        for (std::size_t col = 0; col < width; col++) {
            matrix_.emplace(pos(row, col), ' ');
        }
    }
}

void tui::render::write(TerminalMatrix& matrix, pos start_pos, std::string text) {
    for (size_t i = 0; i < text.size(); i++) {
        matrix[pos(start_pos.x + i, start_pos.y)] = text[i];
    }
}

void tui::render::write(TerminalMatrix& matrix, pos start_pos, char letter) {
    matrix[start_pos] = letter;
}

std::string tui::render::interpret(TerminalMatrix& matrix) {
    std::string result{};
    result.reserve(matrix.size());
    for (std::size_t row = 0; row < matrix.height(); row++) {
        for (std::size_t col = 0; col < matrix.width(); col++) {
            result.push_back(matrix[pos(col, row)]);
        }
        result.push_back('\n');
    }
    return result;
}