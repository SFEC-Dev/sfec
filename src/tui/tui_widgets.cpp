#include "tui.h"

void tui::render_text(vec2d start, std::string text) {
    tui::render::write(current_matrix(), start, text);
}

void tui::render_text_styled(vec2d start, std::string text, Color text_col, Color bg_col, text_flags flags) {
    tui::render::write_styled(current_matrix(), start, text, text_col, bg_col, flags);
}

void tui::widgets::text(std::string text) {
    tui::render::write(current_matrix(), cursor_pos(), text);

    cursor_pos() = vec2d(cursor_pos().x, cursor_pos().y + 1);
}

void selectable(std::string label, bool condition, tui::vec2d size) {
    label.resize(size.x, ' ');

    if (condition)
        tui::render_text_styled(tui::cursor_pos(), label, tui::Color(200, 200, 75), tui::Color(255,255,255));
    else
        tui::render_text(tui::cursor_pos(), label);

    tui::cursor_pos() = tui::vec2d(tui::cursor_pos().x, tui::cursor_pos().y + size.y);
}

void tui::widgets::listbox(int& value, std::vector<std::string> items) {
    tui::cursor_pos() = vec2d(0,0);

    if (binds::get_event(binds::DOWN))
        if (value+1 < items.size())
            value++;

    if (binds::get_event(binds::UP))
        if (value >= 1)
            value--;

    for (size_t i = 0; i < items.size(); i++) {
        selectable(items[i], value == i, vec2d(12, 1));
    }
}
