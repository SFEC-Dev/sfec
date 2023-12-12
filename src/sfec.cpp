#include "tui/tui.h"

#include <string>
#include <sys/ioctl.h>
#include <unistd.h>
#include "tui/utils/icon.h"

int main() {
    using namespace tui;

    struct winsize size;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);

    int buf_size{};
    bool exit = false;

    event_handler event{};
    auto matrix = std::make_unique<render::TerminalMatrix>(size.ws_col, size.ws_row-1, ' ');
    context new_context(&event, std::move(matrix));

    set_context(&new_context);

    system("clear");

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
     
        static int some_value, some_value2, some_value3, some_value4, some_value5;

        std::string bruh(get_window_size().x, 'a');

        for (int i = 0; i < get_window_size().y; i++)
            render_text({0, i}, bruh);

        begin_child("somechild", {22, get_window_size().y});
        widgets::listbox("somelistbox", some_value, items, get_window_size().y);
        end_child();

        set_cursor_pos({23, 0});
        begin_child("somechild2", {22, get_window_size().y});
        widgets::listbox("somelistbox2", some_value2, items, get_window_size().y);
        end_child();

        set_cursor_pos({23 + 23, 0});
        begin_child("somechild3", {22, get_window_size().y});
        widgets::listbox("somelistbox3", some_value3, items, get_window_size().y);
        end_child();

        set_cursor_pos({23 + 23 + 23, 0});
        begin_child("somechild4", {22, get_window_size().y});
        widgets::listbox("somelistbox4", some_value4, items, get_window_size().y);
        end_child();

        set_cursor_pos({23 + 23 + 23 + 23, 0});
        begin_child("somechild5", {22, get_window_size().y});
        widgets::listbox("somelistbox5", some_value5, items, get_window_size().y);
        end_child();

        // render_text_styled({0, current_matrix().height()-1}, "press q to quit | " + std::to_string(event.get_frames()), Color(255, 0, 0), Color(55, 55, 55), FLAG_BOLD | FLAG_ITALIC);
        
        render::draw();

        reset();
    }

    return 0;
}
