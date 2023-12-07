#include "tui/tui.h"

#include <string>
#include <thread>
#include <chrono>
#include <sys/ioctl.h>
#include <unistd.h>

int main() {
    using namespace tui;

    struct winsize size;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);

    int buf_size{};
    bool exit = false;

    key_handler handler{};
    render::TerminalMatrix matrix(size.ws_col, size.ws_row-1, ' ');
    context new_context(&handler, &matrix);

    set_context(&new_context);

    while (!exit) {
        handler.handle();

        if (is_key_pressed(KEY_ESC) || is_key_pressed(KEY_LOWERCASE_Q))
            exit = true;

        render_text({0, current_matrix().height()-1}, "press q to quit");

        draw();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    return 0;
}
