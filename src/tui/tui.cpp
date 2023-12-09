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

tui::vec2d& tui::cursor_pos() {
    return g_tui->last_item_pos;
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
    cursor_pos() = vec2d(0,0);
}

void tui::begin_child() {
    
}

void tui::end_child() {

}