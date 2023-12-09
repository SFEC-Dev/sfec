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

        std::string label = "I am text with ";
        render_text({0,0}, label);
        render_text_styled(pos(label.size(), 0), "no style", Color(55, 225, 55), Color(225, 55, 55), FLAG_ITALIC | FLAG_BOLD | FLAG_UNDERLINED | FLAG_BLINK);
        render_text_styled({0, current_matrix().height()-1}, "press q to quit", Color(255, 0, 0), Color(55, 55, 55), FLAG_BOLD | FLAG_ITALIC);
        render_text({0,2}, icon::get_icon("some.cpp"));

        render::draw();
    }

    return 0;
}
