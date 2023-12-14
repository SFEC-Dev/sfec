#include "tui/tui.h"

#include <string>
#include <sys/ioctl.h>
#include <unistd.h>
#include "tui/utils/icon.h"
#include <locale>
#include <codecvt>

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
     
        static int some_value, some_value2;

        tui::widgets::render_frame({{2,2}, {180, 40}}, tui::FRAME_STYLE::DOUBLE);

        /* begin_child("somechild", {get_window_size().x/5, get_window_size().y}); */
        /* widgets::listbox("somelistbox", some_value, items, get_window_size().y); */
        /* end_child(); */
        /**/
        /* set_cursor_pos({get_window_size().x/5 + 1, 0}); */
        /* begin_child("somechild2", {get_window_size().x/3, get_window_size().y}); */
        /* widgets::listbox("somelistbox2", some_value2, items, get_window_size().y); */
        /* end_child(); */

        ustring quit_message = "\U0001f34c press q to quit"; quit_message.value.resize(get_window_size().x, ' ');
        render_text_styled({0, current_matrix().height()-1}, quit_message, Color(255, 0, 0), Color(55, 55, 55), FLAG_BOLD | FLAG_ITALIC);
        render::draw();

        reset();
    }

    return 0;
}
