#include "tui.h"

tui::key_handler::key_handler() {
    tcgetattr(STDIN_FILENO, &orig_termios);

    struct termios new_termios = orig_termios;
    new_termios.c_lflag &= ~ICANON;
    tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);

    struct winsize terminal_size;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &terminal_size);
}

tui::key_handler::~key_handler() {
    tcsetattr(STDIN_FILENO, TCSANOW, &orig_termios);
}

void tui::key_handler::handle() {
    fd_set read_set;
    FD_ZERO(&read_set);
    FD_SET(STDIN_FILENO, &read_set);
    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 10000;

    if (select(STDIN_FILENO + 1, &read_set, NULL, NULL, &timeout) > 0)
    {
        is_key_pressed = true;

        char input;
        read(STDIN_FILENO, &input, 1);

        get_io().current_key = static_cast<keys>(input);
    } else
        is_key_pressed = false;
}

tui::io& tui::get_io() {
    static io global_io{};
    return global_io;
}