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

    std::cout << "\033[?25l";
    while (!exit) {
        handler.handle();   

        if (is_key_pressed(KEY_ESC) || is_key_pressed(KEY_LOWERCASE_Q))
            exit = true;

        //static int select_id
        //widgets::start_column(select_id);
        //widgets::selectable("text", select_id == 1)
        //widgets;:end_column();


        render_text({0, current_matrix().height()-1}, "press q to quit");

        draw();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    std::cout << "\033[?25h";

    return 0;
}
