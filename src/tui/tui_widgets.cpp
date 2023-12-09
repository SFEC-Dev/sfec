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


void selectable(std::string label, bool condition, tui::pos size) {
    label.resize(size.x, ' ');

    if (condition)
        tui::render_text_styled(tui::g_tui->last_item_pos, label, tui::Color(200, 200, 75), tui::Color(255,255,255));
    else
        tui::render_text(tui::g_tui->last_item_pos, label);

    tui::g_tui->last_item_pos = tui::pos(tui::g_tui->last_item_pos.x, tui::g_tui->last_item_pos.y + size.y);
}

void tui::widgets::listbox(int& value, std::vector<std::string> items) {
    tui::g_tui->last_item_pos = pos(0,0);

    if (binds::get_event(binds::DOWN))
        if (value+1 < items.size())
            value++;

    if (binds::get_event(binds::UP))
        if (value >= 1)
            value--;

    for (size_t i = 0; i < items.size(); i++) {
        selectable(items[i], value == i, pos(12, 1));
    }
}