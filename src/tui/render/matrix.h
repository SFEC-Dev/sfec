#pragma once

#include <string>
#include <vector>

namespace tui {
struct vec2d
{
    vec2d() = default;
    vec2d(int x_, int y_) : x{x_}, y{y_} {};
    int x;
    int y;
};

vec2d operator+(const vec2d& lhs, const vec2d& rhs);
bool operator<(const vec2d& lhs, const vec2d& rhs);
bool operator==(const vec2d& lhs, const vec2d& rhs);

struct rect
{
    rect(vec2d start_, vec2d end_) : start{start_}, end{end_} {};
    vec2d start;
    vec2d end;
};

struct uchar
{
    uchar(const char* letter) : value{letter} {};
    uchar(std::string letter) : value{letter} {};
    uchar() = default;
    std::string value;
};

struct ustring
{
    ustring(size_t n, const uchar& letter) {
        value.reserve(n);
        for (size_t i = 0; i < n; i++) {
            value.append(letter.value);
        }
    }
    ustring(const char* text) : value{text} {};
    ustring(std::string text) : value{text} {};
    std::string value;
};

uchar operator ""_uchr(const char*,size_t);
ustring operator ""_ustr(const char*, size_t);

ustring operator+(const uchar& lhs, const uchar& rhs);
ustring operator+(const uchar& lhs, const ustring& rhs);
ustring operator+(const ustring& lhs, const ustring& rhs);
ustring operator+(const ustring& lhs, const uchar& rhs);

namespace render {
void clear();

class TerminalMatrix
{
    std::vector<std::vector<std::pair<std::string, std::string>>> matrix_;

    int width_;
    int height_;

    char filler_;

public:
    TerminalMatrix(int width, int height, char filler = ' ');
    TerminalMatrix(int width, int height, TerminalMatrix& from);

    std::pair<std::string, std::string>& operator[](vec2d where) {
        return matrix_[where.y][where.x];
    }

    int width() const {
        return width_;
    }

    int height() const {
        return height_;
    }

    char filler() const {
        return filler_;
    }

    int size() const {
        return width_ * height_;
    }

    int real_size() const {
        return matrix_.size();
    }

    using iterator =
        std::vector<std::vector<std::pair<std::string, std::string>>>::iterator;
    using const_iterator = std::vector<
        std::vector<std::pair<std::string, std::string>>>::const_iterator;

    iterator begin() {
        return matrix_.begin();
    }
    iterator end() {
        return matrix_.end();
    }

    const_iterator cbegin() const {
        return matrix_.cbegin();
    }
    const_iterator cend() const {
        return matrix_.cend();
    }
};

void write(TerminalMatrix& matrix, vec2d where,
           std::pair<std::string, std::string> content);
void write(TerminalMatrix& matrix, vec2d where, char letter);
void write(TerminalMatrix& matrix, vec2d start, std::string text);
void write(TerminalMatrix& matrix, vec2d start, TerminalMatrix& from);

void write(TerminalMatrix& matrix, vec2d where, uchar letter);
void write(TerminalMatrix& matrix, vec2d start, ustring text);

void wipe(TerminalMatrix& matrix, vec2d start, vec2d end);

void interpret(TerminalMatrix& matrix);

void draw();
} // namespace render
} // namespace tui
