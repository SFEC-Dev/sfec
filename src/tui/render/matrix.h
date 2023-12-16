#pragma once
#include "../utils/position.h"
#include "color.h"

#include <string>
#include <vector>
#include <algorithm>

namespace tui {
    namespace render {
    class TerminalMatrix {
        int width_;
        int height_;

        char32_t filler_;

        std::vector<std::vector<std::pair<char32_t, std::string>>> matrix_;

    public:
        TerminalMatrix(int width, int height, char32_t filler = U' ');
        TerminalMatrix(int width, int height, TerminalMatrix& from);

        std::pair<char32_t, std::string>& operator[](vec2d where) 
        {
            const auto clamped_pos = vec2d(std::clamp(where.x, 0, width_-1), std::clamp(where.y, 0, height_-1));
            return matrix_[clamped_pos.y][clamped_pos.x];
        }

        void resize(vec2d new_size) {
            height_ = new_size.y;
            width_ = new_size.x;

            matrix_.resize(new_size.y, std::vector<std::pair<char32_t, std::string>>(new_size.x, std::pair<char32_t, std::string>(filler_, "")));

            for (auto& line : matrix_) {
                line.resize(new_size.x, std::pair<char32_t, std::string>(filler_, ""));
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

        using iterator =
            std::vector<std::vector<std::pair<char32_t, std::string>>>::iterator;
        using const_iterator = std::vector<
            std::vector<std::pair<char32_t, std::string>>>::const_iterator;

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
            std::pair<char32_t, std::string> content);
    void write(TerminalMatrix& matrix, vec2d where, char32_t letter);
    void write(TerminalMatrix& matrix, vec2d start, std::u32string_view text);
    void write(TerminalMatrix& matrix, vec2d start, TerminalMatrix& from);
    void write_styled(TerminalMatrix& matrix, vec2d where, char32_t letter, 
                    Color text_col = Color(), Color bg_col = Color(), text_flags flags = 0);
                    
    void write_styled(TerminalMatrix& matrix, vec2d start, std::u32string_view text, 
                        Color text_col = Color(), Color bg_col = Color(), text_flags flags = 0);


    void wipe(TerminalMatrix& matrix, vec2d start, vec2d end);

    void interpret(TerminalMatrix& matrix);
    }
}
