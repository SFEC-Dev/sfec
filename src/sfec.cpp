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
    while (!exit) {
        event.process();

        if (is_key_pressed(KEY_ESC) || is_key_pressed(KEY_LOWERCASE_Q))
            exit = true;
     
        static int some_value;
        widgets::listbox(some_value, {"pivo", "grechka", "plov", "pelmeni", "borsh", "polba"});

        widgets::text("text after child");
        begin_child({16, 8});
        for (size_t i = 1; i <= 12; i++) {
            widgets::text("child text №" + std::to_string(i));
        }
        end_child();
        widgets::text("text before child");

        render_text_styled({0, current_matrix().height()-1}, "press q to quit", Color(255, 0, 0), Color(55, 55, 55), FLAG_BOLD | FLAG_ITALIC);
        
        render::draw();

        reset();
    }

    return 0;
}
