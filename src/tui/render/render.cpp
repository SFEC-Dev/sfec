#include "render.h"
#include "../tui.h"


void tui::render::draw_char(vec2d start, char32_t text) {
    tui::render::write(current_matrix(), start, text);
}

void tui::render::draw_char_styled(vec2d start, char32_t text, Color text_col, Color bg_col, text_flags flags) {
    tui::render::write_styled(current_matrix(), start, text, text_col, bg_col, flags);
}

void tui::render::draw_text(vec2d start, std::u32string text) {
    tui::render::write(current_matrix(), start, text);
}

void tui::render::draw_text_styled(vec2d start, std::u32string text, Color text_col, Color bg_col, text_flags flags) {
    tui::render::write_styled(current_matrix(), start, text, text_col, bg_col, flags);
}

void tui::render::draw() {
    std::cout << "\033[H";
    std::cout.flush();
    render::interpret(current_matrix());
    g_tui->old_matrix = *g_tui->g_matrix;
}

void tui::render::clear() {
    system("clear");
}