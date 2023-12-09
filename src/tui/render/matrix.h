#pragma once

#include <map>
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

    vec2d calculate_pos(vec2d what, int width, int height);

    struct rect {
        rect(vec2d start_, vec2d end_) : start{start_}, end{end_}{};
        vec2d start;
        vec2d end;
    };
namespace render {
    void clear();

    class TerminalMatrix {
        std::map<vec2d, char> matrix_;
        std::map<vec2d, std::pair<vec2d, std::string>> style_;

        int width_;
        int height_;

        char filler_;

        vec2d last_style_pos_;
    public:
        TerminalMatrix(int width, int height, char filler = ' ');
        ~TerminalMatrix();

        char& operator[](vec2d where) {
            return matrix_[calculate_pos(where, width_, height_)];
        }

        int width() const {
            return width_;
        }

        int height() const {
            return height_;
        }

        int filler() const {
            return filler_;
        }

        int size() const {
            return width_ * height_;
        }

        std::map<vec2d, std::pair<vec2d, std::string>>& style() {
            return style_;
        }

        vec2d& last_style_pos() {
            return last_style_pos_;
        }
        
        using iterator = std::map<vec2d, char>::iterator;
        using const_iterator = std::map<vec2d, char>::const_iterator;

        iterator begin() { return matrix_.begin(); }
        iterator end() { return matrix_.end(); }
        
        const_iterator cbegin() const { return matrix_.cbegin(); }
        const_iterator cend() const { return matrix_.cend(); }

    };
    bool busy(TerminalMatrix& matrix, vec2d where);
    bool busy(TerminalMatrix& matrix, vec2d start, vec2d end);

    void write(TerminalMatrix& matrix, vec2d where, char letter);
    void write(TerminalMatrix& matrix, vec2d start, std::string text);

    void wipe(TerminalMatrix& matrix, vec2d start, vec2d end);

    std::string interpret(TerminalMatrix& matrix);

    void draw();
}
}