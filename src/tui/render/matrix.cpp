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

tui::render::TerminalMatrix::TerminalMatrix(int width, int height, wchar_t filler) : width_(width), height_(height), filler_{filler}, matrix_(height, std::vector(width, std::pair<wchar_t, std::wstring>(filler, L""))) {
}

void tui::render::write(TerminalMatrix& matrix, vec2d where, wchar_t letter) {
    matrix[where].first = letter;
}

void tui::render::write(TerminalMatrix& matrix, vec2d where, std::pair<wchar_t, std::wstring> content) {
    matrix[where] = content;
}

void tui::render::write(TerminalMatrix& matrix, vec2d start, std::wstring text) {
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
            if (matrix[vec2d(col, row)].second != L"")
                matrix[vec2d(col, row)].second.clear();
        }
    }
}

const std::wstring end_seq{L"\033[0m"};

void tui::render::interpret(TerminalMatrix& matrix) {
    for (std::size_t row = 0; row < matrix.height(); row++) {
        for (std::size_t col = 0; col < matrix.width(); col++) { 
            if (matrix[vec2d(col, row)] == g_tui->old_matrix[vec2d(col, row)]) {
                continue;
            }
            
            std::wcout << L"\033[" + std::to_wstring(row+1) + L";" + std::to_wstring(col+1) + L"H";

            if (matrix[vec2d(col, row)].second != L"") {
                std::wcout << matrix[vec2d(col, row)].second << matrix[vec2d(col, row)].first << end_seq;
            } 
            else
                std::wcout << matrix[vec2d(col, row)].first;
            
        }
        std::wcout << '\n';
    }
}

void tui::render::clear() {
    system("clear");
}

void tui::render::draw() {
    std::wcout << "\033[H";
    std::wcout.flush();
    render::interpret(current_matrix());
    g_tui->old_matrix = *g_tui->g_matrix;
}