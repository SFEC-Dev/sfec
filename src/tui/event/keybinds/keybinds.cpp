#include "keybinds.h"

std::map<tui::binds::event, tui::binds::key_proxy> tui::binds::current_bindings{
    {DELETE, key_proxy({key_combine({KEY_LOWERCASE_D, KEY_UPPERCASE_D})})},
    {UP, key_proxy({KEY_LOWERCASE_K})},
    {DOWN, key_proxy({KEY_LOWERCASE_J})},
    {LEFT, key_proxy({KEY_LOWERCASE_H})},
    {RIGHT, key_proxy({KEY_LOWERCASE_L})},
    {ADD, key_proxy({KEY_LOWERCASE_A})}};


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

