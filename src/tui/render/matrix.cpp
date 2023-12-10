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
    for (std::size_t row = 0; row < height; row++) {
        for (std::size_t col = 0; col < width; col++) {
            matrix_[vec2d(col, row)].first = filler;
        }
    }
}


tui::render::TerminalMatrix::TerminalMatrix(int width, int height, TerminalMatrix& from) : width_(width), height_(height), filler_{from.filler()} {
    for (std::size_t row = 0; row < height; row++) {
        for (std::size_t col = 0; col < width; col++) {
            matrix_[vec2d(col, row)] = from[vec2d(col, row)];
        }
    }
}


bool tui::render::busy(TerminalMatrix& matrix, vec2d where) {
    return (matrix[where].first == matrix.filler());
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
    matrix[where].first = letter;
}

void tui::render::write(TerminalMatrix& matrix, vec2d where, std::pair<char, std::string> content) {
    matrix[where] = content;
}

void tui::render::write(TerminalMatrix& matrix, vec2d start, std::string text) {
    for (size_t i = 0; i < text.size(); i++) {
        write(matrix, vec2d(start.x + i, start.y), text[i]);
    }
}

void tui::render::write(TerminalMatrix& matrix, vec2d start, TerminalMatrix& from) {
    for (std::size_t row = 0; row < from.height(); row++) {
        for (std::size_t col = 0; col < from.width(); col++) {
             write(matrix, vec2d(col + start.x, row + start.y), from[vec2d(col, row)]);
        }
    }
}

void tui::render::wipe(TerminalMatrix& matrix, vec2d start, vec2d end) {
    for (std::size_t row = start.y; row < end.y; row++) {
        for (std::size_t col = start.x; col < end.x; col++) {
            matrix[vec2d(col, row)].first = matrix.filler();
            if (!matrix[vec2d(col, row)].second.empty())
                matrix[vec2d(col, row)].second.clear();
        }
    }
}

const std::string end_seq{"\033[0m"};

std::string tui::render::interpret(TerminalMatrix& matrix) {
    std::string result{};
    result.reserve(matrix.size());
    for (std::size_t row = 0; row < matrix.height(); row++) {
        for (std::size_t col = 0; col < matrix.width(); col++) { 
            if (!matrix[vec2d(col, row)].second.empty()) {
                result.append(matrix[vec2d(col, row)].second);
                result.push_back(matrix[vec2d(col, row)].first);
                result.append(end_seq);
            } 
            else
                result.push_back(matrix[vec2d(col, row)].first);
            
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
    wipe(current_matrix(), vec2d(0,0), vec2d(current_matrix().width(), current_matrix().height()));
}