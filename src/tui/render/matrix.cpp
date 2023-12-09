#include "matrix.h"

tui::pos tui::operator+(pos lhs, pos rhs) {
    return pos(lhs.x + rhs.x, lhs.y + rhs.y);
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

bool tui::render::busy(TerminalMatrix& matrix, pos where) {
    return (matrix[where] == matrix.filler());
}

bool tui::render::busy(TerminalMatrix& matrix, pos start, pos end) {
    for (std::size_t row = start.y; row < end.y; row++) {
        for (std::size_t col = start.x; col < end.x; col++) {
            if (busy(matrix, pos(col, row)))
                return true;
        }
    }
    return false;
}

void tui::render::write(TerminalMatrix& matrix, pos where, char letter) {
    matrix[where] = letter;
}

void tui::render::write(TerminalMatrix& matrix, pos start, std::string text) {
    for (size_t i = 0; i < text.size(); i++) {
        write(matrix, pos(start.x + i, start.y), text[i]);
    }
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

void tui::render::clear() {
    system("clear");
}
