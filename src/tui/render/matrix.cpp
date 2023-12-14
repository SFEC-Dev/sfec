#include "matrix.h"
#include "../tui.h"

#include <iostream>
#include <locale>
#include <codecvt>

tui::vec2d tui::operator+(const vec2d& lhs, const vec2d& rhs) {
    return vec2d(lhs.x + rhs.x, lhs.y + rhs.y);
}

tui::ustring tui::operator+(const uchar& lhs, const uchar& rhs){
    return tui::ustring(lhs.value + rhs.value);
}

tui::ustring tui::operator+(const ustring& lhs, const uchar& rhs){
    return tui::ustring(lhs.value + rhs.value);
}

tui::ustring tui::operator+(const uchar& lhs, const ustring& rhs){
    return ustring(lhs.value + rhs.value);
}

tui::ustring tui::operator+(const ustring& lhs, const ustring& rhs){
    return tui::ustring(lhs.value + rhs.value);
}

bool tui::operator<(const vec2d& lhs, const vec2d& rhs) {
    return std::tie(lhs.x, lhs.y) < std::tie(rhs.x, rhs.y);
}

bool tui::operator==(const vec2d& lhs, const vec2d& rhs) {
    return std::tie(lhs.x, lhs.y) == std::tie(rhs.x, rhs.y);
}

tui::uchar tui::operator ""_uchr(const char* text, size_t n){
    return uchar(text);
};

tui::ustring tui::operator ""_ustr(const char* text, size_t n){
    return ustring(text);
};

tui::render::TerminalMatrix::TerminalMatrix(int width, int height, char filler) : width_(width), height_(height), filler_{filler}, 
                                            matrix_(height, std::vector(width, std::pair<std::string, std::string>(std::string(filler, 1), ""))) 
{
}

void tui::render::write(TerminalMatrix& matrix, vec2d where, char letter) {
    matrix[where].first = letter;
}

void tui::render::write(TerminalMatrix& matrix, vec2d where, std::pair<std::string, std::string> content) {
    matrix[where] = content;
}

void tui::render::write(TerminalMatrix& matrix, vec2d start, std::string text) {
    for (size_t i = 0; i < text.size(); i++) {
        write(matrix, vec2d(start.x + i, start.y), text[i]);
    }
}

void tui::render::write(TerminalMatrix& matrix, vec2d where, uchar unicode_char) {
    matrix[where].first = unicode_char.value;
}

void tui::render::write(TerminalMatrix& matrix, vec2d start, ustring text) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
    std::wstring wstr = converter.from_bytes(text.value);

    int shift = 0;
    for (auto it = wstr.begin(); it != wstr.end(); ++it) {
        std::wstring singleChar(1, *it);
        std::string utf8Char = converter.to_bytes(singleChar);
        if (utf8Char.size() >= 4) {
            write(matrix, vec2d(start.x + std::distance(wstr.begin(), it) + shift, start.y), uchar{utf8Char});
            shift++;
        } else
            write(matrix, vec2d(start.x + std::distance(wstr.begin(), it) + shift, start.y), uchar{utf8Char});
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
            if (matrix[vec2d(col, row)].second != "")
                matrix[vec2d(col, row)].second.clear();
        }
    }
}

const std::string end_seq{"\033[0m"};

void tui::render::interpret(TerminalMatrix& matrix) {
    for (std::size_t row = 0; row < matrix.height(); row++) {
        for (std::size_t col = 0; col < matrix.width(); col++) { 
            if (matrix[vec2d(col, row)] == g_tui->old_matrix[vec2d(col, row)]) {
                continue;
            }
            
            std::cout << "\033[" + std::to_string(row+1) + ";" + std::to_string(col+1) + "H";

            if (matrix[vec2d(col, row)].second != "") {
                std::cout << matrix[vec2d(col, row)].second << matrix[vec2d(col, row)].first << end_seq;
            } 
            else
                std::cout << matrix[vec2d(col, row)].first;
            
        }
        std::cout << '\n';
    }
}

void tui::render::clear() {
    system("clear");
}

void tui::render::draw() {
    std::cout << "\033[H";
    std::cout.flush();
    render::interpret(current_matrix());
    g_tui->old_matrix = *g_tui->g_matrix;
}
