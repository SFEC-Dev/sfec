#pragma once

#include "key_handler.h"

#include <utility>

namespace tui {
class event_handler {
    key_handler handler_;

    // Cuz we need skip first frame
    int next_frames = 1;
public:
    void process();
    void add_frame();
    void add_frames(int count);

    key_handler& get_key_handler() {
        return handler_;
    }
};

}