#include "event_handler.h"
#include "../tui.h"

#include <thread>
#include <chrono>
#include <sys/ioctl.h>
#include <unistd.h>
#include <csignal>

bool is_resized = false;

tui::event_handler::event_handler() {
    auto handle_resize_signal = [](int) { is_resized = true; };
    signal(SIGWINCH, handle_resize_signal);
};

void tui::event_handler::add_frame() {
    if (next_frames == 0)
        next_frames++;
}

void tui::event_handler::add_frames(int count) {
    if (next_frames < count)
        next_frames = count;
}

int tui::event_handler::get_frames() {
    return next_frames;
}

void handle_resize() {
    struct winsize size;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);

    tui::render::clear();
    tui::render::wipe(tui::current_matrix(), {0, 0}, tui::vec2d(tui::current_matrix().width(), tui::current_matrix().height()));
    tui::current_matrix().resize({size.ws_col, size.ws_row-1});
    //g_tui->g_style.child_pudding = 

    int bytesAvailable;
    ioctl(STDIN_FILENO, FIONREAD, &bytesAvailable);

    if (bytesAvailable > 0) {
        std::cin.ignore();
        is_resized = true;
    } else {
        is_resized = false;
    }
}

void tui::event_handler::process() {
    while (true){    
        handler_.handle();

        if (is_resized) {
            handle_resize();
            break;
        }

        if (next_frames > 0) {
            next_frames--;
            break;
        }

        if (is_any_pressed())
            break;

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    };
}

