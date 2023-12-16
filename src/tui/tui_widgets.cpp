#include "tui.h"

#include <algorithm>
#include <array>

void tui::widgets::text(std::u32string_view text) {
    render::draw_text(get_cursor_pos(), text);

    set_cursor_pos(get_cursor_pos() +
                   vec2d(0, 1 + current_style().item_spacing));
}

void tui::widgets::text_styled(std::u32string_view text, Color text_col, Color bg_col, text_flags flags) {
    render::draw_text_styled(get_cursor_pos(), text, text_col, bg_col, flags);

    set_cursor_pos(get_cursor_pos() +
                   vec2d(0, 1 + current_style().item_spacing));
}

void selectable_impl(tui::icons::icon_t icon, std::u32string_view label, bool condition, tui::vec2d size) {
    using namespace tui;
    
    if (condition) {  //  && g_tui->enable_input
        std::u32string active_text = icon.first + (U' ' + std::u32string(label));
        active_text.resize(size.x, U' ');
        tui::render::draw_text_styled(tui::get_cursor_pos(), active_text, Color(250, 180, 0), Color(120, 50, 0), tui::FLAG_REVERSE | tui::FLAG_BOLD | tui::FLAG_ITALIC);
    }
    else {
        tui::render::draw_char_styled(tui::get_cursor_pos(), icon.first, icon.second);
        tui::render::draw_text(tui::get_cursor_pos() + vec2d(2, 0), label);
    }

    tui::set_cursor_pos(tui::get_cursor_pos() + vec2d(0, size.y + tui::current_style().item_spacing));
}

void selectable(tui::icons::icon_t icon, std::u32string_view label, bool condition, tui::vec2d size) {
    selectable_impl(icon, label, condition, size);
}

void selectable(std::u32string label, bool condition, tui::vec2d size) {
    selectable_impl(tui::icons::icon_t(U' ', tui::Color()), label, condition, size);
}

bool tui::widgets::listbox(const std::string& id, int& value, const std::vector<std::pair<icons::icon_t, std::u32string>>& items, int height, listbox_flags flags) {
    using namespace tui;
    if (binds::get_event(binds::DOWN) && g_tui->enable_input)
        if (value + 1 < items.size())
            value++;

    if (binds::get_event(binds::UP) && g_tui->enable_input)
        if (value >= 1)
            value--;

    const int min = std::min(static_cast<int>(items.size()), height);
    const int max = std::max(static_cast<int>(items.size()), height);

    auto& saved_position =
        (g_tui->listbox_buffer.try_emplace(id, std::pair<int, int>(0, min)).first->second);

    const int scroll_activator = min * 0.25;

    while (saved_position.second - saved_position.first < min) {
        if (saved_position.first > 0)
            saved_position.first--;
        if (saved_position.second - saved_position.first < min && saved_position.second < max)
        saved_position.second++;
    }

    while (saved_position.second - saved_position.first > min) {
        if (saved_position.first > 0)
            saved_position.first++;
        if (saved_position.second - saved_position.first > min)
        saved_position.second--;
    }

    if (height < items.size()) {
    while (value < saved_position.first + scroll_activator && saved_position.first > 0) {
        saved_position.first--;
        saved_position.second--;
    }

    while (value >= saved_position.second - scroll_activator && saved_position.second < max) {
        saved_position.first++;
        saved_position.second++;
    }
    }

    for (size_t i = saved_position.first; i < saved_position.second; i++) {
        selectable(items[i].first, items[i].second, flags & LISTBOX_FLAG_DISABLED ? false : value == i, vec2d(get_window_size().x, 1));
    }

    if (binds::get_event(binds::ADD) && g_tui->enable_input)
        return true;

    return false;
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
    std::array<char32_t, 6> result;
    switch (style) {
        case tui::BORDER_STYLE::ROUND:
            result[0] = tui::converter.from_bytes(ltc_round)[0];
            result[1] = tui::converter.from_bytes(rtc_round)[0];
            result[2] = tui::converter.from_bytes(lbc_round)[0];
            result[3] = tui::converter.from_bytes(rbc_round)[0];
            result[4] = tui::converter.from_bytes(v_line)[0];
            result[5] = tui::converter.from_bytes(line)[0];
            break;
        case tui::BORDER_STYLE::SOLID:
            result[0] = tui::converter.from_bytes(ltc_solid)[0];
            result[1] = tui::converter.from_bytes(rtc_solid)[0];
            result[2] = tui::converter.from_bytes(lbc_solid)[0];
            result[3] = tui::converter.from_bytes(rbc_solid)[0];
            result[4] = tui::converter.from_bytes(v_line)[0];
            result[5] = tui::converter.from_bytes(line)[0];
            break;
        case tui::BORDER_STYLE::DOUBLE:
            result[0] = tui::converter.from_bytes(ltc_double)[0];
            result[1] = tui::converter.from_bytes(rtc_double)[0];
            result[2] = tui::converter.from_bytes(lbc_double)[0];
            result[3] = tui::converter.from_bytes(rbc_double)[0];
            result[4] = tui::converter.from_bytes(v_line_double)[0];
            result[5] = tui::converter.from_bytes(line_double)[0];
            break;
    }
    return result; 
}

void tui::widgets::render_border(rect frame, const tui::BORDER_STYLE style) {
    auto style_frame = get_style_border(style);
    int width = frame.end.x - frame.start.x;
    std::u32string line(width - 1, style_frame[5]);

    tui::render::write(tui::current_matrix(), frame.start,
                       style_frame[0] + line + style_frame[1]);
    tui::render::write(tui::current_matrix(),
                       vec2d(frame.start.x, frame.end.y),
                       style_frame[2] + line + style_frame[3]);

    for (int y = frame.start.y + 1 ; y <= frame.end.y - 1; y++) {
        tui::render::write(tui::current_matrix(), vec2d(frame.start.x, y) ,style_frame[4]);
        tui::render::write(tui::current_matrix(), vec2d(frame.end.x, y), style_frame[4]);
    }
};
