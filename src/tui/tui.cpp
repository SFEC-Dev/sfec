#include <iostream>
#include "tui.h"

tui::terminal::terminal(key_handler* handler, int width, int height, char letter) : 
                                    matrix_{width, height, letter},
                                    handler_{handler} {};

tui::render::TerminalMatrix& tui::terminal::get_matrix() {
    return matrix_;
}

tui::terminal* tui::main = nullptr;

void tui::set_context(terminal* context) {
    main = context;
}

void tui::draw() {
    std::cout << render::interpret(main->get_matrix());
}

tui::keys tui::terminal::get_pressed_key() const {
    return handler_->get_pressed();
}

bool tui::is_any_pressed() {
    return !(main->get_pressed_key() == KEY_NUL);
}

bool tui::is_key_pressed(keys key) {
    return (is_any_pressed() && main->get_pressed_key() == key);
}