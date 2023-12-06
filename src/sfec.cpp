#include <iostream>
#include <string>
#include <thread>
#include <chrono>

#include "tui/tui.h"

void clear_screen() {
    std::cout << "\033[2J\033[1;1H";
}

int main() {
    using namespace tui;

    int buf_size{};
    bool exit = false;
    key_handler handler{};
    terminal new_terminal(&handler, 15, 5, '#');

    set_context(&new_terminal);

    render_text({0, 0}, "press s to wipe");

    while (!exit) {
        clear_screen();
        handler.handle();

        if (tui::is_key_pressed(tui::KEY_ESC) || tui::is_key_pressed(tui::KEY_LOWERCASE_Q))
            exit = true;

        if (tui::is_any_pressed()) 
            render_text({++buf_size, 1}, std::string(1, new_terminal.get_pressed_key()));

        if (tui::is_key_pressed(tui::KEY_LOWERCASE_S)) {
            render::wipe(new_terminal.get_matrix(), {0,0}, {new_terminal.get_matrix().width()/2, new_terminal.get_matrix().height()});
        }

        draw();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    clear_screen();

    return 0;
}
