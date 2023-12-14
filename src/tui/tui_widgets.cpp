#include "tui.h"

#include <algorithm>
#include <array>

void tui::render_text(vec2d start, std::string text) {
    tui::render::write(current_matrix(), start, text);
}

void tui::render_text_styled(vec2d start, std::string text, Color text_col,
                             Color bg_col, text_flags flags) {
    tui::render::write_styled(current_matrix(), start, text, text_col, bg_col,
                              flags);
}

void tui::render_text(vec2d start, ustring text) {
    tui::render::write(current_matrix(), start, text);
}

void tui::render_text_styled(vec2d start, ustring text, Color text_col,
                             Color bg_col, text_flags flags) {
    tui::render::write_styled(current_matrix(), start, text, text_col, bg_col,
                              flags);
}

void tui::widgets::text(std::string text) {
    tui::render::write(current_matrix(), get_cursor_pos(), text);

    set_cursor_pos(get_cursor_pos() +
                   vec2d(0, 1 + current_style().item_spacing));
}

void selectable(std::string label, bool condition, tui::vec2d size) {
    label.resize(size.x, ' ');

    if (condition)
        tui::render_text_styled(
            tui::get_cursor_pos(), label, tui::Color(), tui::Color(),
            tui::FLAG_REVERSE | tui::FLAG_BOLD | tui::FLAG_ITALIC);
    else
        tui::render_text(tui::get_cursor_pos(), label);

    tui::set_cursor_pos(
        tui::get_cursor_pos() +
        tui::vec2d(0, size.y + tui::current_style().item_spacing));
}

void tui::widgets::listbox(std::string id, int& value,
                           std::vector<std::string> items, int height) {
    if (binds::get_event(binds::DOWN) && g_tui->enable_input)
        if (value + 1 < items.size())
            value++;

    if (binds::get_event(binds::UP) && g_tui->enable_input)
        if (value >= 1)
            value--;

    const int min = std::min(static_cast<int>(items.size()), height);
    const int max = std::max(static_cast<int>(items.size()), height);

    int& saved_position =
        (g_tui->listbox_buffer.try_emplace(id, min).first->second);

    if (value > saved_position - min / 4.1 && saved_position < max)
        saved_position++;

    if (value < saved_position - min + min / 4.8 && saved_position > min)
        saved_position--;

    for (size_t i = saved_position - min; i < saved_position; i++) {
        selectable(items[i], value == i, vec2d(get_window_size().x, 1));
    }
}

constexpr const char* v_line        = "\u2502", * v_line_double = "\u2551",
                    * line          = "\u2500",
                    * line_double   = "\u2550";

constexpr const char* ltc_round     = "\u256D",
                    * rtc_round     = "\u256E",
                    * lbc_round     = "\u2570",
                    * rbc_round     = "\u256F";

constexpr const char* ltc_solid     = "\u250C",
                    * rtc_solid     = "\u2510",
                    * lbc_solid     = "\u2514",
                    * rbc_solid     = "\u2518";

constexpr const char* ltc_double = "\u2554",
                    * rtc_double = "\u2557",
                    * lbc_double = "\u255A",
                    * rbc_double = "\u255D";

 auto get_style_border(tui::BORDER_STYLE style){
    std::array<tui::uchar,6> result;
    switch (style) {
        case tui::BORDER_STYLE::ROUND:
            result = { ltc_round,rtc_round,lbc_round,rbc_round,v_line,line };
            break;
        case tui::BORDER_STYLE::SOLID:
            result = { ltc_solid,rtc_solid,lbc_solid,rbc_solid,v_line,line };
            break;
        case tui::BORDER_STYLE::DOUBLE:
            result = { ltc_double, rtc_double, lbc_double, rbc_double,v_line_double, line_double };
            break;
    }
    return result; 
}

void tui::widgets::render_border(tui::rect frame, const tui::BORDER_STYLE style) {
    auto style_frame = get_style_border(style);
    int width = frame.end.x - frame.start.x;
    ustring line(width - 1, style_frame[5]);

    tui::render::write(tui::current_matrix(), frame.start,
                       style_frame[0] + line + style_frame[1]);
    tui::render::write(tui::current_matrix(),
                       tui::vec2d(frame.start.x, frame.end.y),
                       style_frame[2] + line + style_frame[3]);

    for (int y = frame.start.y + 1 ; y <= frame.end.y - 1; y++) {
        tui::render::write(tui::current_matrix(), tui::vec2d(frame.start.x, y) ,style_frame[4]);
        tui::render::write(tui::current_matrix(), tui::vec2d(frame.end.x, y), style_frame[4]);
    }
};
