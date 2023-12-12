#pragma once

#include <vector>
#include <string>

namespace tui {
    struct vec2d {
        vec2d() = default;
        vec2d(int x_, int y_) : x{x_}, y{y_}{};
        int x;
        int y;
    };

    vec2d operator+(const vec2d& lhs, const vec2d& rhs);
    bool operator<(const vec2d& lhs, const vec2d& rhs);
    bool operator==(const vec2d& lhs, const vec2d& rhs);

    struct rect {
        rect(vec2d start_, vec2d end_) : start{start_}, end{end_}{};
        vec2d start;
        vec2d end;
    };

namespace render {
    void clear();

    class TerminalMatrix {
        std::vector<std::vector<std::pair<wchar_t, std::wstring>>> matrix_;

        int width_;
        int height_;

        wchar_t filler_;

    public:
        TerminalMatrix(int width, int height, wchar_t filler = ' ');
        TerminalMatrix(int width, int height, TerminalMatrix& from);

        std::pair<wchar_t, std::wstring>& operator[](vec2d where) {
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
        
        using iterator = std::vector<std::vector<std::pair<wchar_t, std::wstring>>>::iterator;
        using const_iterator = std::vector<std::vector<std::pair<wchar_t, std::wstring>>>::const_iterator;

        iterator begin() { return matrix_.begin(); }
        iterator end() { return matrix_.end(); }
        
        const_iterator cbegin() const { return matrix_.cbegin(); }
        const_iterator cend() const { return matrix_.cend(); }

    };

    void write(TerminalMatrix& matrix, vec2d where, std::pair<wchar_t, std::wstring> content);
    void write(TerminalMatrix& matrix, vec2d where, wchar_t letter);
    void write(TerminalMatrix& matrix, vec2d start, std::wstring text);
    void write(TerminalMatrix& matrix, vec2d start, TerminalMatrix& from);

    void wipe(TerminalMatrix& matrix, vec2d start, vec2d end);

    void interpret(TerminalMatrix& matrix);

    void draw();
}
}