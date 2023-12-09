#pragma once

#include "../../tui.h"
#include "../key_handler.h"
#include <map>
#include <variant>
#include <vector>

namespace tui::binds {

enum event
{
    UP,
    DOWN,
    LEFT,
    RIGHT,
    RENAME,
    YANK,
    PASTE,
    DELETE,
    ADD
};

struct key_combine
{
    std::vector<keys> key_sequence;

    key_combine(std::vector<keys> combine) {
        key_sequence = std::move(combine);
    }
};

struct key_proxy
{
    using proxy_type = std::vector<std::variant<keys, key_combine>>;

    proxy_type proxy;

    key_proxy(proxy_type new_proxy) {
        proxy = std::move(new_proxy);
    }
    using iterator = proxy_type::iterator;
    using const_iterator = proxy_type::const_iterator;

    iterator begin() {
        return proxy.begin();
    }
    iterator end() {
        return proxy.end();
    }

    const_iterator cbegin() const {
        return proxy.cbegin();
    }
    const_iterator cend() const {
        return proxy.cend();
    }
};

extern std::map<event, key_proxy> current_bindings;
bool get_event(event action);

}
