#include "tui/tui.h"

#include <chrono>
#include <string>
#include <sys/ioctl.h>
#include <thread>
#include <unistd.h>

int main() {
    using namespace tui;

    struct winsize size;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);

    int buf_size{};
    bool exit = false;

    key_handler handler{};
    render::TerminalMatrix matrix(size.ws_col, size.ws_row - 1, ' ');
    context new_context(&handler, &matrix);

    set_context(&new_context);

    std::cout << "\033[?25l";
    system("clear");
    while (!exit) {
        handler.handle();

        render_text({0, ++buf_size}, "I love Text");
        ;

        if (is_key_pressed(KEY_ESC) || is_key_pressed(KEY_LOWERCASE_Q))
            exit = true;

        render_text({0, 0}, is_any_pressed() ? "true" : "false");
        render_text({0, current_matrix().height() - 1}, "press q to quit");

        std::cout << "\033[H";
        std::cout.flush();

        draw();

        while (true) {
            handler.handle();

            if (is_any_pressed())
                break;

            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }

    std::cout << "\033[?25h";

    return 0;
}
