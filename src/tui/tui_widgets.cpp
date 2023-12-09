#include "tui.h"

void tui::render_text(pos start, std::string text) {
    tui::render::write(current_matrix(), start, text);
}

void tui::render_text_styled(pos start, std::string text, Color text_col, Color bg_col, text_flags flags) {
    tui::render::write_styled(current_matrix(), start, text, text_col, bg_col, flags);
}

void tui::widgets::text(std::string text) {
    rect text_rect(g_tui->last_item_pos, g_tui->last_item_pos + pos(text.size(), 0));

    g_tui->last_item_pos = pos(0, text_rect.end.y + current_style().item_spacing);

    tui::render::write(current_matrix(), text_rect.start, text);
}