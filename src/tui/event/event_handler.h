#pragma once

#include "key_handler.h"

namespace tui {
class event_handler {
    key_handler handler_;

    // Cuz we need skip first frames
    int next_frames = 2;
    bool terminal_resize = false;

public:
    event_handler();

    void process();
    void add_frame();
    void add_frames(int count);
    int get_frames();

    key_handler& get_key_handler() {
        return handler_;
    }
};

}