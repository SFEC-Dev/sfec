#include "tui/tui.h"

#include <string>
#include <sys/ioctl.h>
#include <unistd.h>
#include <filesystem>
#include <locale>
#include <codecvt>
#include <set>

int main(int argc, char* argv[]) {
    using namespace tui;

    bool exit = false;
    std::filesystem::path current_path = std::filesystem::current_path();

    // std::vector<std::u32string> current_items;
    // for (auto const& dir_entry : std::filesystem::directory_iterator{current_path}) 
    //     current_items.emplace_back(dir_entry.path().filename().u32string());

    // for (size_t i = 0; i < current_items.size(); i++) {
    //     std::cout << converter.to_bytes(current_items[i]) + " " + std::to_string(i) << std::endl;
    // }
    // return 2;

    struct winsize start_size;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &start_size);

    event_handler event{};
    auto matrix = std::make_unique<render::TerminalMatrix>(start_size.ws_col, start_size.ws_row-1, U' ');
    context new_context(&event, std::move(matrix));

    set_context(&new_context);
    set_min_window_size({64, 24});
    int some_value = 0, some_value2 = 0;

    try {
    while (!exit) {
        event.process();

        if (is_key_pressed(KEY_ESC) || is_key_pressed(KEY_LOWERCASE_Q))
            throw std::runtime_error("Nigger died...");
            //exit = true;

        if (is_key_pressed(tui::KEY_LOWERCASE_H) && g_tui->active_child > 0)
            g_tui->active_child--;

        if (is_key_pressed(tui::KEY_LOWERCASE_L) && g_tui->active_child < g_tui->true_childs.size() - 1)
            g_tui->active_child++;

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

        std::filesystem::path parent_path = current_path.parent_path();
        std::vector<std::u32string> parent_items;
        for (const auto& dir_entry : std::filesystem::directory_iterator{parent_path}) 
            parent_items.emplace_back(dir_entry.path().filename().u32string());

        std::vector<std::u32string> current_items;
        for (const auto& dir_entry : std::filesystem::directory_iterator{current_path}) 
            current_items.push_back(dir_entry.path().filename().u32string());

        std::sort(parent_items.begin(), parent_items.end());
        std::sort(current_items.begin(), current_items.end());

        set_cursor_pos({0, get_window_size().y/5});
        begin_child("parent", {get_window_size().x/5, get_window_size().y - get_window_size().y/5 - 5}, true);
        widgets::listbox("parent_files", some_value, parent_items, get_window_size().y);
        end_child();

        set_cursor_pos({get_window_size().x/5 + 1, get_window_size().y/5});
        begin_child("current", {get_window_size().x/3, get_window_size().y - get_window_size().y/5 - 5}, true);
        widgets::listbox("current_files", some_value2, current_items, get_window_size().y);
        end_child();

        std::u32string quit_message{U"press \U0001f34c to quit"};
        quit_message.resize(get_window_size().x, U' ');
        render_text_styled({0, current_matrix().height()-1}, quit_message, Color(250, 180, 0), Color(120, 50, 0), FLAG_REVERSE | FLAG_ITALIC);
       
        render::draw();
        reset();
    }
    }
    catch (const std::exception& e) {
        std::cout << std::to_string(some_value2) << std::endl;
        std::cout << std::to_string(g_tui->listbox_buffer["somelistbox2"].first) << std::endl;
        std::cout << std::to_string(g_tui->listbox_buffer["somelistbox2"].second) << std::endl;
        std::cout << e.what() << std::endl;
    }

    return 0;
}
