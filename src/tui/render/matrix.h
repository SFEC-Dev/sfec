#include <map>
#include <string>
#include <algorithm>

namespace tui {
    struct pos {
        pos(int x_, int y_) : x{x_}, y{y_}{};
        int x;
        int y;
    };

    pos operator+(pos lhs, int rhs);
    bool operator<(pos lhs, pos rhs);

    struct rect {
        rect(pos start_, pos end_) : start{start_}, end{end_}{};
        pos start;
        pos end;
    };
namespace render {
    class TerminalMatrix {
        std::map<pos, char> matrix_;

        int width_;
        int height_;

        char filler_;
    public:
        TerminalMatrix(int width, int height, char filler = ' ');

        char& operator[](pos where) {
            while (where.x >= width_) {
                where.x -= width_;
                where.y++;
            }
            return matrix_[pos(where.x, where.y % height_)];
            // if ((where.x < width_) && (where.y < height_)) 
            //     return matrix_[where];

           // return matrix_[pos(where.x - static_cast<int>(where.x / width_) * width_, where.y - static_cast<int>(where.y / height_) * height_)];
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

        using iterator = std::map<pos, char>::iterator;
        using const_iterator = std::map<pos, char>::const_iterator;

        iterator begin() { return matrix_.begin(); }
        iterator end() { return matrix_.end(); }
        
        const_iterator cbegin() const { return matrix_.cbegin(); }
        const_iterator cend() const { return matrix_.cend(); }

    };

    void write(TerminalMatrix& matrix, pos start, std::string text);
    void write(TerminalMatrix& matrix, pos start, char letter);

    void wipe(TerminalMatrix& matrix, pos start, pos end);

    std::string interpret(TerminalMatrix& matrix);
}
}