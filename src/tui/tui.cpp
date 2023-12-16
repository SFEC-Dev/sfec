#include "tui.h"

#include <algorithm>

tui::context* tui::g_tui = nullptr;

void tui::set_context(tui::context* new_context) {
    g_tui = std::move(new_context);
}
tui::render::TerminalMatrix& tui::current_matrix() {
    return *g_tui->g_matrix;    
}

tui::event_handler& tui::current_event() {
    return *g_tui->g_event;
}

tui::style& tui::current_style() {
    return g_tui->g_style;
}

void tui::set_min_window_size(vec2d size) {
    g_tui->minimal_window_size = size;
}

void tui::set_cursor_pos(vec2d where) {
    g_tui->last_item_pos = where;
}   

const tui::vec2d& tui::get_cursor_pos() {
    return g_tui->last_item_pos;
}

const tui::vec2d tui::get_window_size() {
    return vec2d(current_matrix().width(), current_matrix().height());
}

tui::keys tui::get_pressed_key() {
    return g_tui->g_event->get_key_handler().get_pressed();
}

bool tui::is_any_pressed() {
    return !(get_pressed_key() == KEY_NUL);
}

bool tui::is_key_pressed(keys key) {
    return (is_any_pressed() && get_pressed_key() == key);
}

void tui::reset() {
    g_tui->child_buffer.clear();
    set_cursor_pos(vec2d(0, 0));
}

void tui::begin_child(const std::string& id, vec2d size, bool border) {
    const auto true_childs = g_tui->true_childs;
   
    if (auto it = std::find(true_childs.cbegin(), true_childs.cend(), id); it != true_childs.cend() ) {
        int index = std::distance(true_childs.cbegin(), it);
        if (index == g_tui->active_child)
            g_tui->enable_input = true;
    }

    g_tui->child_buffer.push_back({id, g_tui->child_buffer.size()+1});

    const vec2d padding = current_style().child_padding;

    vec2d matrix_size{std::clamp(size.x - padding.x*2, 0, get_window_size().x - get_cursor_pos().x), 
                      std::clamp(size.y - padding.y*2, 0, get_window_size().y - get_cursor_pos().y - 1)};

    if (border) {
        vec2d border_pos {std::clamp(get_cursor_pos().x + size.x, 0, get_window_size().x), 
                      std::clamp(get_cursor_pos().y + size.y - 1, 0, get_window_size().y-1)};

        widgets::render_border({get_cursor_pos(), border_pos}, current_style().child_border_style);
    }

    auto& last_pos = g_tui->last_child_pos;
    vec2d this_pos = get_cursor_pos() + padding;
    
    if (std::find(last_pos.cbegin(), last_pos.cend(), this_pos) == last_pos.cend())
        last_pos.emplace_back(this_pos);
                      
    auto child_matrix = std::make_unique<render::TerminalMatrix>(matrix_size.x, matrix_size.y, U' ');

    set_cursor_pos(vec2d(0, 0));

    g_tui->matrix_buffer = std::move(g_tui->g_matrix);
    g_tui->g_matrix = std::move(child_matrix);
}

void tui::end_child() { 
    auto& true_childs = g_tui->true_childs;

    if (g_tui->child_buffer.back().second == g_tui->child_buffer.size()) {
        if (std::find(true_childs.cbegin(), true_childs.cend(), g_tui->child_buffer.back().first) == true_childs.cend())
            true_childs.push_back(g_tui->child_buffer.back().first);

         g_tui->enable_input = false;
    }

    g_tui->child_buffer.pop_back();

    const vec2d padding = current_style().child_padding;
    auto& last_pos = g_tui->last_child_pos;

    render::write((*g_tui->matrix_buffer), last_pos.back(), (*g_tui->g_matrix));
    last_pos.pop_back();

    set_cursor_pos(vec2d(0, padding.y*2 + (*g_tui->g_matrix).height() + tui::current_style().item_spacing));

    g_tui->g_matrix = std::move(g_tui->matrix_buffer);

}
