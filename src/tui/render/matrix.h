#pragma once

#include <map>
#include <string>

namespace tui {
    struct pos {
        pos() = default;
        pos(int x_, int y_) : x{x_}, y{y_}{};
        int x;
        int y;
    };

    pos operator+(const pos& lhs, const pos& rhs);
    bool operator<(const pos& lhs, const pos& rhs);
    bool operator==(const pos& lhs, const pos& rhs);

    pos calculate_pos(pos what, int width, int height);

    struct rect {
        rect(pos start_, pos end_) : start{start_}, end{end_}{};
        pos start;
        pos end;
    };
namespace render {
    void clear();

    class TerminalMatrix {
        std::map<pos, char> matrix_;
        std::map<pos, std::pair<pos, std::string>> style_;

        int width_;
        int height_;

        char filler_;

        pos last_style_pos_;
    public:
        TerminalMatrix(int width, int height, char filler = ' ');
        ~TerminalMatrix();

        char& operator[](pos where) {
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

        std::map<pos, std::pair<pos, std::string>>& style() {
            return style_;
        }

        pos& last_style_pos() {
            return last_style_pos_;
        }
        
        using iterator = std::map<pos, char>::iterator;
        using const_iterator = std::map<pos, char>::const_iterator;

        iterator begin() { return matrix_.begin(); }
        iterator end() { return matrix_.end(); }
        
        const_iterator cbegin() const { return matrix_.cbegin(); }
        const_iterator cend() const { return matrix_.cend(); }

    };
    bool busy(TerminalMatrix& matrix, pos where);
    bool busy(TerminalMatrix& matrix, pos start, pos end);

    void write(TerminalMatrix& matrix, pos where, char letter);
    void write(TerminalMatrix& matrix, pos start, std::string text);

    void wipe(TerminalMatrix& matrix, pos start, pos end);

    std::string interpret(TerminalMatrix& matrix);

    void draw();
}
}