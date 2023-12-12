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
        int item_spacing = 0;
        int child_pudding = 4;
    };

    struct context {
        context(event_handler* handler, std::unique_ptr<render::TerminalMatrix> matrix) :
                g_event{std::move(handler)}, g_matrix{std::move(matrix)}, old_matrix{*g_matrix} {
        
            // For hidden cursor
            std::cout << "\033[?25l";
        };

        ~context() {
            // For show cursor
            std::cout << "\033[?25h";
            render::clear();
        }

        event_handler* g_event{nullptr};
        std::unique_ptr<render::TerminalMatrix> g_matrix{nullptr};
        style g_style{};

        vec2d last_item_pos{0, 0};
        std::vector<vec2d> last_child_pos{};
        render::TerminalMatrix old_matrix;
        int active_child{0};
        bool enable_input{false};
        
        std::vector<tui::keys> key_buffer{};
        std::chrono::steady_clock::time_point buffer_time{};
        std::unique_ptr<render::TerminalMatrix> matrix_buffer{};
        std::map<std::string, int> listbox_buffer{};
        std::vector<std::pair<std::string, int>> child_buffer{};
        std::vector<std::string> true_childs{};
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
        void listbox(std::string id, int& value, std::vector<std::string> items, int height);
    }

    void reset();

    void begin_child(std::string id, vec2d size);     
    void end_child();
}

