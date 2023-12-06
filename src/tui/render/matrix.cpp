
#include "matrix.h"

tui::pos tui::operator+(tui::pos lhs, int rhs) {
    lhs.x += rhs;
    lhs.y += rhs;
    return lhs;
}

bool tui::operator<(pos lhs, pos rhs) {
    return std::tie(lhs.x, lhs.y) < std::tie(rhs.x, rhs.y);
}

tui::render::TerminalMatrix::TerminalMatrix(int width, int height, char filler) : width_(width), height_(height), filler_{filler} {
    for (std::size_t row = 0; row < height; row++) {
        for (std::size_t col = 0; col < width; col++) {
            matrix_.emplace(pos(col, row), filler);
        }
    }
}

void tui::render::write(TerminalMatrix& matrix, pos start, std::string text) {
    for (size_t i = 0; i < text.size(); i++) {
        matrix[pos(start.x + i, start.y)] = text[i];
    }
}

void tui::render::write(TerminalMatrix& matrix, pos start, char letter) {
    matrix[start] = letter;
}

void tui::render::wipe(TerminalMatrix& matrix, pos start, pos end) {
    for (std::size_t row = start.y; row < end.y; row++) {
        for (std::size_t col = start.x; col < end.x; col++) {
            matrix[pos(col, row)] = matrix.filler();
        }
    }
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