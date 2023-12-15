#include "tui/tui.h"

#include <string>
#include <sys/ioctl.h>
#include <unistd.h>
#include <csignal>

int main() {
    using namespace tui;

    bool exit = false;

    struct winsize start_size;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &start_size);

    event_handler event{};
    auto matrix = std::make_unique<render::TerminalMatrix>(start_size.ws_col, start_size.ws_row-1, ' ');
    context new_context(&event, std::move(matrix));

    set_context(&new_context);

    std::vector<std::string> items;

    for (size_t i = 0; i < 64; i++) {
        items.push_back("item " + std::to_string(items.size() + 1));
    }
    
    while (!exit) {
        event.process();

        if (is_key_pressed(KEY_ESC) || is_key_pressed(KEY_LOWERCASE_Q))
            exit = true;

        if (is_key_pressed(tui::KEY_LOWERCASE_H) && g_tui->active_child > 0)
            g_tui->active_child--;

        if (is_key_pressed(tui::KEY_LOWERCASE_L) && g_tui->active_child < g_tui->true_childs.size() - 1)
            g_tui->active_child++;
     
        static int some_value, some_value2;
        static int items_height;

        std::vector<std::string> sfec_logo = {
        "███████ ███████ ███████  ██████ ",
        "██      ██      ██      ██      ",
        "███████ █████   █████   ██      ",
        "     ██ ██      ██      ██      ",
        "███████ ██      ███████  ██████ "
        };
        if (get_window_size().x > 39 + 2 && get_window_size().y/4-1 > sfec_logo.size() + 2) {
            set_cursor_pos({(get_window_size().x - 39) / 2, 2});
            for (int i = 0; i < sfec_logo.size(); i++) {
                widgets::text_styled(ustring{sfec_logo[i]}, Color(250, 180, 0));
               // render_text_styled({get_window_size().x - 10, i + 3}, ustring{sfecnana[i]}, Color(250, 180, 0));
            }
        }

        set_cursor_pos({0, get_window_size().y/5});
        begin_child("somechild", {get_window_size().x/5, get_window_size().y - get_window_size().y/5 - 5}, true);
        widgets::listbox("somelistbox", some_value, items, get_window_size().y);
        end_child();

        set_cursor_pos({get_window_size().x/5 + 1, get_window_size().y/5});
        begin_child("somechild2", {get_window_size().x/3, get_window_size().y - get_window_size().y/5 - 5}, true);
        widgets::listbox("somelistbox2", some_value2, items, get_window_size().y);
        end_child();

        ustring quit_message{"press \U0001f34c to quit"};
        quit_message().resize(get_window_size().x, ' ');
        render_text_styled({0, current_matrix().height()-1}, quit_message, Color(250, 180, 0), Color(120, 50, 0), FLAG_REVERSE | FLAG_ITALIC);
        render::draw();

        reset();
    }

    return 0;
}
