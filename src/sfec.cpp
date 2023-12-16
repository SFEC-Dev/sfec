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

        std::vector<files::fs::directory_entry> parent_items;
        for (const auto& dir_entry : files::fs::directory_iterator{parent_path}) 
            parent_items.emplace_back(dir_entry);

        std::vector<files::fs::directory_entry> current_items;
        for (const auto& dir_entry : files::fs::directory_iterator{current_path}) 
            current_items.emplace_back(dir_entry);

        files::default_sort(parent_items);
        files::default_sort(current_items);

        std::vector<std::pair<icons::icon_t, std::u32string>> parent_names(parent_items.size());
        std::vector<std::pair<icons::icon_t, std::u32string>> current_names(current_items.size());

        std::transform(parent_items.begin(), parent_items.end(), parent_names.begin(), [](const files::fs::directory_entry& entry){ 
            return std::pair<icons::icon_t, std::u32string>(icons::get_icon(entry), entry.path().filename().u32string());
        });

        std::transform(current_items.begin(), current_items.end(), current_names.begin(), [](const files::fs::directory_entry& entry) { 
            return std::pair<icons::icon_t, std::u32string>(icons::get_icon(entry), entry.path().filename().u32string());
        });

        if (!current_path.has_relative_path())
            parent_names.clear();

//TODO: ADD MORE ICONS!
//TODO: ADD MORE BINDS!

        static std::stack<int> parent_idx({0});
        static int current_idx{0}, filled{0};

        const int parent_width = get_window_size().x/5;
        const int current_width = get_window_size().x/3;
        const int preview_width = get_window_size().x/3;
        const int childs_y_pos = get_window_size().y/5;
        const vec2d childs_indendation{2, 2};
        const int childs_height = get_window_size().y - childs_y_pos - childs_indendation.y;

        set_cursor_pos({2, childs_y_pos});
        begin_child("parent", {parent_width, childs_height}, true);
        widgets::listbox("parent_files", parent_idx.top(), parent_names, get_window_size().y);
        end_child();

        set_cursor_pos({2 + parent_width + childs_indendation.x, childs_y_pos});
        begin_child("current", {current_width, childs_height}, true);
        if (widgets::listbox("current_files", current_idx, current_names, get_window_size().y)) {
            if (current_items[current_idx].is_directory()) {
                parent_path = current_path;
                current_path = current_items[current_idx].path();
                parent_idx.push(current_idx);
                current_idx = 0;
                event.add_frame();
            }
        }
        end_child();

        if (is_key_pressed(tui::KEY_LOWERCASE_H) && current_path.has_relative_path()) {
            current_path = parent_path;
            if (parent_path.has_parent_path())
                parent_path = parent_path.parent_path();
            current_idx = parent_idx.top();
            if (parent_idx.size() > 1)
                parent_idx.pop();
            event.add_frame();
        } else {

        set_cursor_pos({2 + parent_width + current_width + childs_indendation.x * 2, childs_y_pos});
        begin_child("preview", {preview_width, childs_height}, true);
        if (current_items[current_idx].is_directory()) {
            std::vector<files::fs::directory_entry> preview_items;
            for (const auto& dir_entry : files::fs::directory_iterator{current_items[current_idx]}) 
                preview_items.emplace_back(dir_entry);

            files::default_sort(preview_items);
            std::vector<std::pair<icons::icon_t, std::u32string>> preview_names(preview_items.size());
            std::transform(preview_items.begin(), preview_items.end(), preview_names.begin(), [](const files::fs::directory_entry& entry){ 
                return std::pair<icons::icon_t, std::u32string>(icons::get_icon(entry), entry.path().filename().u32string());
            });

            widgets::listbox("directory_preview", filled, preview_names, get_window_size().y, LISTBOX_FLAG_DISABLED);
        }
        else {
            // Other preview implementations
        }
        end_child();
        }
        std::u32string quit_message{current_path.u32string()};
        quit_message.resize(get_window_size().x, U' ');
        render::draw_text_styled({0, current_matrix().height()-1}, quit_message, Color(250, 180, 0), Color(120, 50, 0), FLAG_REVERSE | FLAG_ITALIC);
       
        render::draw();
        reset();
    }

    return 0;
}
