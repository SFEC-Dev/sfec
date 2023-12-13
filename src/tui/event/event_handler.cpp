#include "event_handler.h"
#include "../tui.h"

#include <thread>
#include <chrono>

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

void tui::event_handler::process(bool& iff) {
    while (true){    
        handler_.handle();

        if (next_frames > 0) {
            next_frames--;
            break;
        }

        if (iff)
            break;

        if (is_any_pressed())
            break;

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    };
}

