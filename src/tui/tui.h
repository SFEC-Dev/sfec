#pragma once
#include <memory>
#include <iostream>

#include "event/event_handler.h"
#include "event/key_handler.h"
#include "render/matrix.h"

namespace tui {
    struct style {
        int item_spacing = 4;
    };

    struct context {
        context(event_handler* handler, render::TerminalMatrix* matrix) :
                g_event{std::move(handler)}, g_matrix{std::move(matrix)} {};

        pos last_item_pos;
        

        event_handler* g_event;
        render::TerminalMatrix* g_matrix;
        style g_style;
    };

    extern context* g_tui;

    void set_context(context* new_context);

    render::TerminalMatrix& current_matrix();
    event_handler& current_event();
    style& current_style();

    keys get_pressed_key();
    bool is_key_pressed(const keys key);
    bool is_any_pressed();

    void render_text(pos start, std::string text);
    
    namespace widgets {
        void text(std::string text);
    }
}

