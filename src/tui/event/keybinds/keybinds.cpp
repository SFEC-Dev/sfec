#include "../../tui.h"
#include <chrono>
#include <vector>

std::map<tui::binds::event, tui::binds::key_proxy> tui::binds::current_bindings{
    {UP, key_proxy({KEY_LOWERCASE_K})},
    {DOWN, key_proxy({KEY_LOWERCASE_J})},
    {PREV, key_proxy({KEY_LOWERCASE_H})},
    {NEXT, key_proxy({KEY_LOWERCASE_L})}};

bool check_binds(tui::binds::key_combine combine) {
    auto& buffer = tui::g_tui->key_buffer;
    auto& future = tui::g_tui->buffer_time;

    if (std::chrono::steady_clock::now() <= future) {
        tui::current_event().add_frame();
    }

    if (std::chrono::steady_clock::now() >= future && !buffer.empty()) {
        buffer.clear();
        return false;
    }

    if (!tui::is_any_pressed()) {
        return false;
    }

    if (buffer.empty()) {
        if (is_key_pressed(combine[0])) {
            buffer.push_back(combine[0]);
            future = std::chrono::steady_clock::now() + std::chrono::seconds(3);
        }
    } else {
        if ((buffer[buffer.size() - 1] == combine[buffer.size() - 1]) &&
            is_key_pressed(combine[buffer.size()])) {
            future = std::chrono::steady_clock::now() + std::chrono::seconds(3);
            buffer.push_back(combine[buffer.size()]);
        } else {
            buffer.clear();
            return false;
        }

        if (buffer == combine.key_sequence) {
            buffer.clear();
            return true;
        }
    }

    return false;
}

bool tui::binds::get_event(event action) {
    auto bind = current_bindings.find(action);
    if (bind == current_bindings.cend())
        return false;

    for (const auto& keys_variant : bind->second) {
        if (const keys* key = std::get_if<keys>(&keys_variant)) {
            if (is_key_pressed(*key))
                return true;
        } else if (const key_combine* key =
                       std::get_if<key_combine>(&keys_variant)) {
            if (check_binds(*key)) {
                return true;
                ;
            }
        }
    }
    return false;
}
