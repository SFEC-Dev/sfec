#pragma once
#include "../utils/position.h"
#include "color.h"

#include <string>

namespace tui {
    namespace render {
        void draw_char(vec2d start, char32_t text);
        void draw_char_styled(vec2d start, char32_t text, Color text_col = Color(), Color bg_col = Color(), text_flags flags = 0);

        void draw_text(vec2d start, std::u32string_view text);
        void draw_text_styled(vec2d start, std::u32string_view text, Color text_col = Color(), Color bg_col = Color(), text_flags flags = 0);

        void draw();
        void clear();
    }
}