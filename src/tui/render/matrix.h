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
    bool operator>(const vec2d& lhs, const vec2d& rhs);
    bool operator==(const vec2d& lhs, const vec2d& rhs);

    struct rect {
        rect(vec2d start_, vec2d end_) : start{start_}, end{end_}{};
        vec2d start;
        vec2d end;
    };

    struct uchar {
        explicit uchar(const char* letter) : value{letter} {};
        explicit uchar(std::string letter) : value{letter} {};
        
        std::string& operator()() {
            return value;
        };

        const std::string& operator()() const {
            return value;
        };

    private:
        std::string value;
    };

    struct ustring {
        ustring(size_t n, const uchar& letter) {
            value.reserve(n);
            for (size_t i = 0; i < n; i++) {
                value.append(letter());
            }
        }

        explicit ustring(const char* text) : value{text} {};
        explicit ustring(std::string text) : value{text} {};

        std::string& operator()() {
            return value;
        };

        const std::string& operator()() const {
            return value;
        };

    private:
        std::string value;
    };

    ustring operator+(const ustring& lhs, const ustring& rhs);

    uchar operator ""_uchr(const char*, size_t);
    ustring operator ""_ustr(const char*, size_t);

namespace render {
    void clear();

    class TerminalMatrix {
        int width_;
        int height_;

        char filler_;

        std::vector<std::vector<std::pair<std::string, std::string>>> matrix_;

    public:
        TerminalMatrix(int width, int height, char filler = ' ');
        TerminalMatrix(int width, int height, TerminalMatrix& from);

        std::pair<std::string, std::string>& operator[](vec2d where) {
            return matrix_[where.y][where.x];
        }

        void resize(vec2d new_size) {
            height_ = new_size.y;
            width_ = new_size.x;

            matrix_.resize(new_size.y, std::vector<std::pair<std::string, std::string>>(new_size.x, std::pair<std::string, std::string>(std::string(1, filler_), "")));

            for (auto& line : matrix_) {
                line.resize(new_size.x, std::pair<std::string, std::string>(std::string(1, filler_), ""));
            }
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
        
        using iterator = std::vector<std::vector<std::pair<std::string, std::string>>>::iterator;
        using const_iterator = std::vector<std::vector<std::pair<std::string, std::string>>>::const_iterator;

        iterator begin() { return matrix_.begin(); }
        iterator end() { return matrix_.end(); }
        
        const_iterator cbegin() const { return matrix_.cbegin(); }
        const_iterator cend() const { return matrix_.cend(); }

    };

    void write(TerminalMatrix& matrix, vec2d where, std::pair<std::string, std::string> content);
    void write(TerminalMatrix& matrix, vec2d where, char letter);
    void write(TerminalMatrix& matrix, vec2d start, std::string text);
    void write(TerminalMatrix& matrix, vec2d start, TerminalMatrix& from);

    void write(TerminalMatrix& matrix, vec2d where, uchar letter);
    void write(TerminalMatrix& matrix, vec2d start, ustring text);

    void wipe(TerminalMatrix& matrix, vec2d start, vec2d end);

    void interpret(TerminalMatrix& matrix);

    void draw();
}
}