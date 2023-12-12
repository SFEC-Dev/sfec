#include "tui.h"

#include <algorithm>

void tui::render_text(vec2d start, std::string text) {
    tui::render::write(current_matrix(), start, text);
}

void tui::render_text_styled(vec2d start, std::string text, Color text_col, Color bg_col, text_flags flags) {
    tui::render::write_styled(current_matrix(), start, text, text_col, bg_col, flags);
}

void tui::widgets::text(std::string text) {
    tui::render::write(current_matrix(), get_cursor_pos(), text);

    set_cursor_pos(get_cursor_pos() + vec2d(0, 1 + current_style().item_spacing));
}

void selectable(std::string label, bool condition, tui::vec2d size) {
    label.resize(size.x, ' ');

    if (condition)
        tui::render_text_styled(tui::get_cursor_pos(), label, tui::Color(), tui::Color(), tui::FLAG_REVERSE | tui::FLAG_BOLD | tui::FLAG_ITALIC);
    else
        tui::render_text(tui::get_cursor_pos(), label);

    tui::set_cursor_pos(tui::get_cursor_pos() + tui::vec2d(0, size.y + tui::current_style().item_spacing));
}

void tui::widgets::listbox(std::string id, int& value, std::vector<std::string> items, int height) {
    if (binds::get_event(binds::DOWN) && g_tui->enable_input)
        if (value+1 < items.size())
            value++;

    if (binds::get_event(binds::UP) && g_tui->enable_input)
        if (value >= 1)
            value--;

    const int min = std::min(static_cast<int>(items.size()), height);
    const int max = std::max(static_cast<int>(items.size()), height);

    int& saved_position = (g_tui->listbox_buffer.try_emplace(id, min).first->second);

    if (value > saved_position - min/4.1  && saved_position < max) 
        saved_position++;

    if (value < saved_position - min + min/4.8 && saved_position > min) 
        saved_position--;

    for (size_t i = saved_position - min; i < saved_position; i++) {
        selectable(items[i], value == i, vec2d(get_window_size().x, 1));
    }
}
