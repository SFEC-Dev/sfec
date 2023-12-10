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
    render::TerminalMatrix matrix(size.ws_col, size.ws_row-1, ' ');
    context new_context(&event, &matrix);

    set_context(&new_context);

    system("clear");

    while (!exit) {
        event.process();

        if (is_key_pressed(KEY_ESC) || is_key_pressed(KEY_LOWERCASE_Q))
            exit = true;

     
     
        static int some_value;
        widgets::listbox(some_value, {"pivo", "grechka", "plov", "pelmeni", "borsh", "polba"});
        render_text({0, 35}, std::to_string(some_value));
        render_text({0, 30}, std::to_string(g_tui->key_buffer.size()));
     
     
        render_text_styled({0, current_matrix().height()-1}, "press q to quit", Color(255, 0, 0), Color(55, 55, 55), FLAG_BOLD | FLAG_ITALIC);

        
        render::draw();
        reset();
    }

    return 0;
}
