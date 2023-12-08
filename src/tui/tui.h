#pragma once
#include <memory>
#include <iostream>

#include "event/key_handler.h"
#include "render/matrix.h"

namespace tui {
    struct style {
        int item_spacing = 4;
    };

    struct context {
        context(key_handler* handler, render::TerminalMatrix* matrix) :
                                    g_handler{std::move(handler)}, g_matrix{std::move(matrix)} {};

        pos last_item_pos;
        
        style g_style;
        key_handler* g_handler;
        render::TerminalMatrix* g_matrix;
    };

    extern context* g_tui;

    void set_context(context* new_context);

    keys get_pressed_key();
    render::TerminalMatrix& current_matrix();
    style& current_style();

    void draw();

    bool is_key_pressed(const keys key);
    bool is_any_pressed();

    void render_text(pos start, std::string text);
    
    namespace widgets {
        void text(std::string text);
    }
}

