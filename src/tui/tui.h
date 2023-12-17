#pragma once
#include "utils/position.h"
#include "event/keybinds/keybinds.h"
#include "event/event_handler.h"
#include "event/key_handler.h"
#include "render/render.h"
#include "render/matrix.h"
#include "render/color.h"
#include "utils/icon.h"

#include <chrono>
#include <iostream>
#include <memory>
#include <locale>
#include <codecvt>

namespace tui {

    enum class BORDER_STYLE {
        DOUBLE,
        SOLID,
        ROUND,
    };

    enum LISTBOX_FLAGS {
        LISTBOX_FLAG           = 0,
        LISTBOX_FLAG_DISABLED  = 1 << 0     
    };

    typedef int listbox_flags;

    struct style {
        BORDER_STYLE child_border_style{BORDER_STYLE::ROUND};
        int item_spacing{0};
        vec2d child_padding{2, 1};
    };

    struct context {
        context(event_handler* handler, std::unique_ptr<render::TerminalMatrix> matrix) :
                g_event{std::move(handler)}, g_matrix{std::move(matrix)}, old_matrix{*g_matrix} {
        
            // For hidden cursor
            std::cout << "\033[?25l";
            render::clear();
        };

        ~context() {
            // For show cursor
            std::cout << "\033[?25h";
            render::clear();
        }

        event_handler* g_event{nullptr};
        std::unique_ptr<render::TerminalMatrix> g_matrix{nullptr};
        style g_style{};

        vec2d minimal_window_size{};
        vec2d last_item_pos{0, 0};
        std::vector<vec2d> last_child_pos{};
        render::TerminalMatrix old_matrix;
        int active_child{1};
        bool enable_input{false};
        bool is_resizing{false};
        
        std::vector<tui::keys> key_buffer{};
        std::chrono::steady_clock::time_point buffer_time{};
        std::unique_ptr<render::TerminalMatrix> matrix_buffer{};
        std::map<std::string, std::pair<int, int>> listbox_buffer{};
        std::vector<std::pair<std::string, int>> child_buffer{};
        std::vector<std::string> true_childs{};
    };
    static std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;

    extern context* g_tui;

    void set_context(context* new_context);

    render::TerminalMatrix& current_matrix();
    event_handler& current_event();
    style& current_style();

    void set_min_window_size(const vec2d size);
    void set_cursor_pos(const vec2d where);
    const vec2d& get_cursor_pos();

    const vec2d get_window_size();

    keys get_pressed_key();
    bool is_key_pressed(const keys key);
    bool is_any_pressed();

    namespace widgets {
        void text(std::u32string text);
        void text_styled(std::u32string text, Color text_col = Color(), Color bg_col = Color(), text_flags flags = 0);

        void text_vertical(std::vector<std::u32string> text);
        void text_vertical_styled(std::vector<std::u32string> text, Color text_col = Color(), Color bg_col = Color(), text_flags flags = 0);

        bool listbox(const std::string& id, int& value, const std::vector<std::pair<icons::icon_t, std::u32string>>& items, int height, listbox_flags flags = 0);
        void render_border(rect frame,  const tui::BORDER_STYLE style = BORDER_STYLE::ROUND);
    }

    void reset();

    void begin_child(const std::string& id, vec2d size, bool frame = false);     
    void end_child();
}

