#include <iostream>
#include "tui.h"

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
    set_cursor_pos(vec2d(0, 0));
}

void tui::begin_child(vec2d size) {
    g_tui->last_child_pos = get_cursor_pos();

    set_cursor_pos(vec2d(0, 0));

    auto child_matrix = std::make_unique<render::TerminalMatrix>(size.x, size.y, ' ');

    g_tui->matrix_buffer = std::move(g_tui->g_matrix);
    g_tui->g_matrix = std::move(child_matrix);
}

void tui::end_child() {
    render::write((*g_tui->matrix_buffer), g_tui->last_child_pos, (*g_tui->g_matrix));

    set_cursor_pos(g_tui->last_child_pos + vec2d(0, (*g_tui->g_matrix).height()));

    g_tui->g_matrix = std::move(g_tui->matrix_buffer);

}