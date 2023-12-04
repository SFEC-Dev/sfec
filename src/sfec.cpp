#include <ios>
#include <iostream>
#include <string>
#include <chrono>

#include "config/options.h"
#include "tui/helpers/style.h"
#include "tui/tui.h"
#include "tui/event/key_handler.h"
#include "config/interpreter.h"

void clear_screen() {
    std::cout << "\033[2J\033[1;1H";
}

int main() {

    //return 0;


    // Example of using tui/helpers/style
    // std::string str = "Hello world!";
    // std::cout << tui::stylize(str, tui::FLAG_BOLD) << std::endl;
    // std::cout <<  tui::stylize(str, tui::FLAG_BLINK) << std::endl;
    // std::cout << tui::stylize(str, tui::FLAG_HIDDEN) << std::endl;
    // std::cout << tui::colorize(tui::stylize(str, tui::FLAG_ITALIC | tui::FLAG_BLINK), tui::Color(255, 55, 55)) << std::endl;
    // std::cout << tui::colorize(str, tui::Color(255, 12, 12)) << std::endl;
    // std::cout << tui::colorize(str, tui::Color(), tui::Color(255, 55, 55)) << std::endl;

    tui::key_handler handler{};
    bool active = true;
    std::string buf{};
    std::string strange_line{};

    auto start_time = std::chrono::steady_clock::now();
    while (active) {
        handler.handle();
        if (tui::is_key_pressed(tui::KEY_ESC) || tui::is_key_pressed(tui::KEY_LOWERCASE_Q))
            active = false;
        
        auto current_time = std::chrono::steady_clock::now();

        if (tui::is_any_pressed())
            buf += tui::get_io().current_key;

        std::cout << "Write your buffer: ";
        //std::cout << buf << std::endl;
        //std::cout << "Current pressed key: " << std::to_string(tui::get_io().current_key) << std::endl;
        std::cout << strange_line << std::endl;

         if (std::chrono::duration_cast<std::chrono::milliseconds>(current_time - start_time).count() >= 500) {
            strange_line += "!";
            start_time = current_time;
        }

        clear_screen();

    }

    return 0;
}
