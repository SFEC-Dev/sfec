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

    event_handler event{};
    render::TerminalMatrix matrix(size.ws_col, size.ws_row-1, ' ');
    context new_context(&event, &matrix);

    set_context(&new_context);

    system("clear");

    while (!exit) {
        event.process();

        if (is_key_pressed(KEY_ESC) || is_key_pressed(KEY_LOWERCASE_Q))
            exit = true;

        render_text({0,0}, is_any_pressed() ? "true" : "false");
        render_text({0, current_matrix().height()-1}, "press q to quit");

        render::draw();
    }

    return 0;
}
