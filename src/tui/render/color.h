#pragma once
#include "matrix.h"

#include <algorithm>

namespace tui {
    struct Color {
        Color(int rt, int gt, int bt) : r{std::clamp(rt, 0, 255)}, 
                                        g{std::clamp(gt, 0, 255)}, 
                                        b{std::clamp(bt, 0, 255)} {};

        Color(){ is_empty = true; };
        int r;
        int g;
        int b;

        bool is_empty = false;
    };

    enum FLAGS {
        FLAG_NONE        = 0,
        FLAG_BOLD        = 1 << 0,
        FLAG_ITALIC      = 1 << 1,
        FLAG_DIM         = 1 << 2,
        FLAG_UNDERLINED  = 1 << 3,
        FLAG_BLINK       = 1 << 4,
        FLAG_REVERSE     = 1 << 5,
        FLAG_HIDDEN      = 1 << 6         
    };

    typedef int text_flags;

    namespace render {
        void write_styled(TerminalMatrix& matrix, vec2d where, char letter, 
                          Color text_col = Color(), Color bg_col = Color(), text_flags flags = 0);
                          
        void write_styled(TerminalMatrix& matrix, vec2d start, std::wstring text, 
                          Color text_col = Color(), Color bg_col = Color(), text_flags flags = 0);

        namespace color {
            std::wstring get_style(Color text_col = Color(), Color bg_col = Color(), text_flags flags = 0);
        }
    }
}