#pragma once
#include "event/keybinds/keybinds.h"
#include "event/event_handler.h"
#include "event/key_handler.h"
#include "render/matrix.h"
#include "render/color.h"

#include <chrono>
#include <iostream>
#include <memory>

namespace tui {
    struct style {
        int item_spacing = 4;
    };

    struct context {
        context(event_handler* handler, std::unique_ptr<render::TerminalMatrix> matrix) :
                g_event{std::move(handler)}, g_matrix{std::move(matrix)} {
        
            // For hidden cursor
            std::cout << "\033[?25l";
        };

        ~context() {
            // For show cursor
            std::cout << "\033[?25h";
            render::clear();
        }

        vec2d last_item_pos;
        vec2d last_child_pos;

        std::vector<tui::keys>key_buffer;

        std::chrono::steady_clock::time_point buffer_time;

        event_handler* g_event;
        std::unique_ptr<render::TerminalMatrix> g_matrix;
        style g_style;

        std::unique_ptr<render::TerminalMatrix> matrix_buffer;
    };

    extern context* g_tui;

    void set_context(context* new_context);

    render::TerminalMatrix& current_matrix();
    event_handler& current_event();
    style& current_style();

    void set_cursor_pos(vec2d where);
    const vec2d& get_cursor_pos();

    const vec2d get_window_size();

    keys get_pressed_key();
    bool is_key_pressed(const keys key);
    bool is_any_pressed();

    void render_text(vec2d start, std::string text);
    void render_text_styled(vec2d start, std::string text, Color text_col = Color(), Color bg_col = Color(), text_flags flags = 0);
    
    namespace widgets {
        void text(std::string text);
        void listbox(int& value, std::vector<std::string> items);
    }

    void reset();

    void begin_child(vec2d size);     

    void end_child();
}

