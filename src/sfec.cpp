#include <iostream>
#include <string>
#include <thread>
#include <chrono>

#include "tui/tui.h"
#include "tui/event/key_handler.h"
#include "tui/render/matrix.h"


void clear_screen() {
    std::cout << "\033[2J\033[1;1H";
}

int main() {
    using namespace tui;

    bool exit = false;
    tui::key_handler handler{};
    render::TerminalMatrix matrix(20, 20);

    render::write(matrix, {0, 0}, "Hello sfec!");

    int buf_size{};

    while (!exit) {
        clear_screen();
        handler.handle();

        if (tui::is_key_pressed(tui::KEY_ESC) || tui::is_key_pressed(tui::KEY_LOWERCASE_Q))
            exit = true;

        if (tui::is_any_pressed()) 
            render::write(matrix, {++buf_size, 0}, tui::get_io().current_key);

        std::cout << render::interpret(matrix);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    return 0;
}
