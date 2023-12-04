#include "key_handler.h"

#include <sys/types.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>

tui::key_handler::key_handler() {
    tcgetattr(STDIN_FILENO, &orig_termios);

    struct termios new_termios = orig_termios;
    new_termios.c_lflag &= ~(ICANON | ECHO);
    
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
        char input;
        read(STDIN_FILENO, &input, 1);

        get_io().current_key = static_cast<keys>(input);
    } else {
        get_io().current_key = KEY_NUL;
    }
}

bool tui::is_any_pressed() {
    return !(get_io().current_key == KEY_NUL);
}

bool tui::is_key_pressed(keys key) {
    return (is_any_pressed() && get_io().current_key == key);
}