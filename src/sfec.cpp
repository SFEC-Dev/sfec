#include "tui/tui.h"
#include "file/file.h"

#include <string>
#include <stack>
#include <sys/ioctl.h>
#include <unistd.h>
#include <filesystem>
#include <algorithm>

int main(int argc, char* argv[]) {
    using namespace tui;

    bool exit = false;
    std::filesystem::path current_path = std::filesystem::current_path();
    files::fs::path parent_path = current_path.parent_path();

    struct winsize start_size;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &start_size);

    event_handler event{};
    auto matrix = std::make_unique<render::TerminalMatrix>(start_size.ws_col, start_size.ws_row-1, U' ');
    context new_context(&event, std::move(matrix));

    set_context(&new_context);
    set_min_window_size({64, 24});

    while (!exit) {
        event.process();

        if (is_key_pressed(KEY_ESC) || is_key_pressed(KEY_LOWERCASE_Q))
            exit = true;

        // For future childs move function
        // if (is_key_pressed(tui::KEY_LOWERCASE_L) && g_tui->active_child < g_tui->true_childs.size() - 1)
        //     g_tui->active_child++;

        if (tui::g_tui->minimal_window_size.y != 0 && (get_window_size().x < tui::g_tui->minimal_window_size.x || 
                                                      get_window_size().y <  tui::g_tui->minimal_window_size.y)) {
            std::cout << "\033[H";
            std::string resize_info = "minsize(" + std::to_string(get_window_size().x - tui::g_tui->minimal_window_size.x) + "," +
                                    std::to_string(get_window_size().y - tui::g_tui->minimal_window_size.y) + ")";
            std::cout << resize_info << std::endl;
            continue;
        }

        std::vector<std::u32string> sfec_logo = {
        U"███████ ███████ ███████  ██████ ",
        U"██      ██      ██      ██      ",
        U"███████ █████   █████   ██      ",
        U"     ██ ██      ██      ██      ",
        U"███████ ██      ███████  ██████ v.alpha"
        };
        if (get_window_size().x > 39 + 2 && get_window_size().y/4-1 > sfec_logo.size() + 2) {
            set_cursor_pos({(get_window_size().x - 39) / 2, 2});
            for (int i = 0; i < sfec_logo.size(); i++) {
                widgets::text_styled(sfec_logo[i], Color(250, 180, 0));
            }
        }

        std::vector<files::fs::directory_entry> parent_items = std::move(files::get_files(parent_path));
        std::vector<files::fs::directory_entry> current_items = std::move(files::get_files(current_path));

        std::vector<std::pair<icons::icon_t, std::u32string>> parent_names = (current_path.has_relative_path() ? std::move(files::get_names(parent_items)) : std::vector<std::pair<icons::icon_t, std::u32string>>());
        std::vector<std::pair<icons::icon_t, std::u32string>> current_names = std::move(files::get_names(current_items));

        const int parent_width = get_window_size().x/5;
        const int current_width = get_window_size().x/3;
        const int preview_width = get_window_size().x/2.5;
        const vec2d childs_indendation{1, 2};
        const int childs_width = parent_width + current_width + preview_width + childs_indendation.x * 2;

        const vec2d childs_pos{(get_window_size().x - childs_width)/2, get_window_size().y/5};
        const int childs_height{ get_window_size().y - childs_pos.y - childs_indendation.y };

        static int current_idx, preview_idx;
        static std::stack<int> saved_idxs({0});
        bool is_upped{ false };
        bool is_downed{ false };

        set_cursor_pos({childs_pos.x, childs_pos.y});
        begin_child("parent", {parent_width, childs_height}, true);
        widgets::listbox("parent_files", saved_idxs.top(), parent_names, get_window_size().y);
        end_child();

        set_cursor_pos({childs_pos.x + parent_width + childs_indendation.x, childs_pos.y});
        begin_child("current", {current_width, childs_height}, true);
        if (widgets::listbox("current_files", current_idx, current_names, get_window_size().y)) {
            if (is_directory(current_items[current_idx])) {
                parent_path = current_path;
                current_path = current_items[current_idx];

                current_items = std::move(files::get_files(current_path));
                current_names = std::move(files::get_names(current_items));

                saved_idxs.push(current_idx);
                current_idx = 0;

                event.add_frame();
                is_downed = true;
            }
        }
        end_child();

        if (!is_downed && binds::get_event(tui::binds::PREV) && current_path.has_relative_path()) {
            current_path = parent_path;

            if (parent_path.has_parent_path())
                parent_path = parent_path.parent_path();

            current_idx = saved_idxs.top();
            if (saved_idxs.size() > 1)
                saved_idxs.pop();

            event.add_frame();
            is_upped = true;
        } 
        
        set_cursor_pos({childs_pos.x + parent_width + current_width + childs_indendation.x * 2, childs_pos.y});
        begin_child("preview", {preview_width, childs_height}, true);

        std::vector<files::fs::directory_entry> preview_items;
        
        if (is_downed && !current_items.empty() && current_items[current_idx].is_directory()) {;
            preview_items = std::move(files::get_files(current_items[current_idx]));

            const std::vector<std::pair<icons::icon_t, std::u32string>> preview_names = std::move(files::get_names(preview_items));

            widgets::listbox("directory_preview", preview_idx, preview_names, get_window_size().y, LISTBOX_FLAG_DISABLED);
        }

        else if (is_upped && !parent_items.empty() && parent_items[current_idx].is_directory()) {;
            preview_items = std::move(files::get_files(parent_items[current_idx]));

            const std::vector<std::pair<icons::icon_t, std::u32string>> preview_names = std::move(files::get_names(preview_items));

            widgets::listbox("directory_preview", preview_idx, preview_names, get_window_size().y, LISTBOX_FLAG_DISABLED);
        }
        else if (!current_items.empty() && current_items[current_idx].is_directory()) {
            preview_items = std::move(files::get_files(current_items[current_idx]));

            const std::vector<std::pair<icons::icon_t, std::u32string>> preview_names = std::move(files::get_names(preview_items));

            widgets::listbox("directory_preview", preview_idx, preview_names, get_window_size().y, LISTBOX_FLAG_DISABLED);
        } 
        else if (!current_items.empty()) {
            widgets::text_vertical_styled(files::read_file(current_items[current_idx]));
        }

        end_child();

        std::u32string quit_message{current_path.u32string()};
        quit_message.resize(get_window_size().x, U' ');
        render::draw_text_styled({0, current_matrix().height()-1}, quit_message, Color(250, 180, 0), Color(120, 50, 0), FLAG_REVERSE | FLAG_ITALIC | FLAG_BOLD);
       
        render::draw();
        reset();
    }
    return 0;
}
