#include "tui/tui.h"

#include <string>
#include <sys/ioctl.h>
#include <unistd.h>
#include <csignal>

bool terminalResized = false;

void handleResizeSignal(int) {
    terminalResized = true;
}

int main() {
    using namespace tui;

    signal(SIGWINCH, handleResizeSignal);

    int buf_size{};
    bool exit = false;

    event_handler event{};
    struct winsize start_size;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &start_size);

    auto matrix = std::make_unique<render::TerminalMatrix>(start_size.ws_col, start_size.ws_row-1, ' ');
    context new_context(&event, std::move(matrix));

    set_context(&new_context);

    system("clear");

    std::vector<std::string> items;

    for (size_t i = 0; i < 64; i++) {
        items.push_back("itemddd " + std::to_string(items.size() + 1));
    }
    while (!exit) {
        event.process(terminalResized);

        if (is_key_pressed(KEY_ESC) || is_key_pressed(KEY_LOWERCASE_Q))
            exit = true;

        if (is_key_pressed(tui::KEY_LOWERCASE_H) && g_tui->active_child > 0)
            g_tui->active_child--;
     
        if (terminalResized) {
            struct winsize size;
            ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);


            render::clear();
            render::wipe(current_matrix(), {0, 0}, vec2d(current_matrix().width(), current_matrix().height()));
            current_matrix().resize({size.ws_col, size.ws_row-1});
            //g_tui->g_style.child_pudding = 

            int bytesAvailable;
            ioctl(STDIN_FILENO, FIONREAD, &bytesAvailable);

            if (bytesAvailable > 0) {
                std::cin.ignore();
                terminalResized = true;
            } else {
                terminalResized = false;
            }
        }
       
        static int some_value, some_value2;

        std::vector<std::string> sfecnana = {
        " ______     ______   ______     ______     __   __     ______     __   __     ______    ",
        "/\\  ___\\   /\\  ___\\ /\\  ___\\   /\\  ___\\   /\\ \"-.\\ \\   /\\  __ \\   /\\ \"-.\\ \\   /\\  __ \\   ",
        "\\ \\___  \\  \\ \\  __\\ \\ \\  __\\   \\ \\ \\____  \\ \\ \\-.  \\  \\ \\  __ \\  \\ \\ \\-.  \\  \\ \\  __ \\  ",
        " \\/\\_____\\  \\ \\_\\    \\ \\_____\\  \\ \\_____\\  \\ \\_\\\\\"\\_\\  \\ \\_\\ \\_\\  \\ \\_\\\\\"\\_\\  \\ \\_\\ \\_\\ ",
        "  \\/_____/   \\/_/     \\/_____/   \\/_____/   \\/_/ \\/_/   \\/_/\\/_/   \\/_/ \\/_/   \\/_/\\/_/ "
        };
        if (vec2d(get_window_size().x, get_window_size().y) > vec2d(sfecnana[3].size(), sfecnana.size())) {
            for (int i = 0; i < sfecnana.size(); i++) {
                render_text_styled({(get_window_size().x - int(sfecnana[i].size()))/2, i + 3}, ustring{sfecnana[i]}, Color(250, 180, 0));
            }
        }

        set_cursor_pos({0, get_window_size().y/5});
        begin_child("somechild", {get_window_size().x/5, get_window_size().y - get_window_size().y/5 - 1}, true);
        widgets::listbox("somelistbox", some_value, items, get_window_size().y);
        end_child();

        set_cursor_pos({get_window_size().x/5 + 1, get_window_size().y/5});
        begin_child("somechild2", {get_window_size().x/3, get_window_size().y - get_window_size().y/5 - 1});
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
