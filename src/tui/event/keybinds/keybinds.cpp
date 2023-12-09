#pragma once

#include "keybinds.h"

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
            return false;
        }
    }
    return false;
}

