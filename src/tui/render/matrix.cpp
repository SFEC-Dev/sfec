#include "matrix.h"
#include "../tui.h"

#include <iostream>

tui::vec2d tui::operator+(const vec2d& lhs, const vec2d& rhs) {
    return vec2d(lhs.x + rhs.x, lhs.y + rhs.y);
}

bool tui::operator<(const vec2d& lhs, const vec2d& rhs) {
    return std::tie(lhs.x, lhs.y) < std::tie(rhs.x, rhs.y);
}

bool tui::operator==(const vec2d& lhs, const vec2d& rhs) {
    return std::tie(lhs.x, lhs.y) == std::tie(rhs.x, rhs.y);
}

tui::vec2d tui::calculate_pos(vec2d what, int width, int height) {
    return vec2d(what.x % width, (what.y + static_cast<int>(what.x / width)) % height);
}

tui::render::TerminalMatrix::TerminalMatrix(int width, int height, char filler) : width_(width), height_(height), filler_{filler} {
    // For hidden cursor
    std::cout << "\033[?25l";

    for (std::size_t row = 0; row < height; row++) {
        for (std::size_t col = 0; col < width; col++) {
            matrix_.emplace(vec2d(col, row), filler);
        }
    }
}

tui::render::TerminalMatrix::~TerminalMatrix() {
    // For show cursor
    std::cout << "\033[?25h";

    clear();
}

bool tui::render::busy(TerminalMatrix& matrix, vec2d where) {
    return (matrix[where] == matrix.filler());
}

bool tui::render::busy(TerminalMatrix& matrix, vec2d start, vec2d end) {
    for (std::size_t row = start.y; row < end.y; row++) {
        for (std::size_t col = start.x; col < end.x; col++) {
            if (busy(matrix, vec2d(col, row)))
                return true;
        }
    }
    return false;
}

void tui::render::write(TerminalMatrix& matrix, vec2d where, char letter) {
    matrix[where] = letter;
}

void tui::render::write(TerminalMatrix& matrix, vec2d start, std::string text) {
    for (size_t i = 0; i < text.size(); i++) {
        write(matrix, vec2d(start.x + i, start.y), text[i]);
    }
}

void tui::render::wipe(TerminalMatrix& matrix, vec2d start, vec2d end) {
    for (std::size_t row = start.y; row < end.y; row++) {
        for (std::size_t col = start.x; col < end.x; col++) {
            matrix[vec2d(col, row)] = matrix.filler();
        }
    }
}

const std::string end_seq{"\033[0m"};

std::string tui::render::interpret(TerminalMatrix& matrix) {
    std::string result{};
    result.reserve(matrix.size());
    for (std::size_t row = 0; row < matrix.height(); row++) {
        for (std::size_t col = 0; col < matrix.width(); col++) {
            auto is_style = matrix.style().find(vec2d(col, row));
            if (is_style != matrix.style().cend()) {
                result.append(is_style->second.second);
                matrix.last_style_pos() = calculate_pos(is_style->second.first, matrix.width(), matrix.height());
            }

            if (vec2d(col, row) == matrix.last_style_pos())
                 result.append(end_seq);

            result.push_back(matrix[vec2d(col, row)]);
            
        }
        result.push_back('\n');
    }
    return result;
}

void tui::render::clear() {
    system("clear");
}

void tui::render::draw() {
    std::cout << "\033[H";
    std::cout.flush();
    std::cout << render::interpret(current_matrix());
    current_matrix().style().clear();
}