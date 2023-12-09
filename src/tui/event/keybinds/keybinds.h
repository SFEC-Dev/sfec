#pragma once

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
    
    keys operator[](int n){
        return key_sequence[n];
    }

    key_combine(std::vector<keys> combine) {
        key_sequence = std::move(combine);
    }
    using const_iterator = std::vector<keys>::const_iterator;
    using iterator = std::vector<keys>::iterator;

    const_iterator begin()  {
        return key_sequence.begin();
    }

    const_iterator end()   {
        return key_sequence.end();
    }

    const_iterator cbegin() const {
        return key_sequence.cbegin();
    }

    const_iterator cend() const  {
        return key_sequence.cend();
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

    iterator begin()  {
        return proxy.begin();
    }
    iterator end()  {
        return proxy.end();
    }
};

extern std::map<event, key_proxy> current_bindings;
bool get_event(event action);

} // namespace tui::binds
