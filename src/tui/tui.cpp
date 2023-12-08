#include <iostream>
#include "tui.h"

tui::context* tui::g_tui = nullptr;

void tui::set_context(tui::context* new_context) {
    g_tui = std::move(new_context);
}

tui::keys tui::get_pressed_key() {
    return g_tui->g_handler->get_pressed();
}

tui::render::TerminalMatrix& tui::current_matrix() {
    return *g_tui->g_matrix;
}

tui::style& tui::current_style() {
    return g_tui->g_style;
}

void tui::draw() {
    render::clear();
    std::cerr << render::interpret(current_matrix());
}

bool tui::is_any_pressed() {
    return !(get_pressed_key() == KEY_NUL);
}

bool tui::is_key_pressed(keys key) {
    return (is_any_pressed() && get_pressed_key() == key);
}