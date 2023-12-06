#pragma once
#include <memory>
#include <iostream>

#include "event/key_handler.h"
#include "render/matrix.h"

namespace tui {
    class terminal {
        key_handler* handler_;
        render::TerminalMatrix matrix_;

    public:
        terminal(key_handler* handler, int width, int height, char letter = ' ');

        keys get_pressed_key() const;
        render::TerminalMatrix& get_matrix();

    };

    extern terminal* main;

    void set_context(terminal* context);

    void draw();

    bool is_key_pressed(const keys key);
    bool is_any_pressed();

    void render_text(pos start, std::string text);
}

