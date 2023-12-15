#pragma once

namespace tui {
    struct vec2d
    {
        vec2d() = default;
        vec2d(int x_, int y_) : x{x_}, y{y_} {};
        int x;
        int y;
    };

    vec2d operator+(const vec2d& lhs, const vec2d& rhs);
    bool operator<(const vec2d& lhs, const vec2d& rhs);
    bool operator>(const vec2d& lhs, const vec2d& rhs);
    bool operator==(const vec2d& lhs, const vec2d& rhs);

    struct rect
    {
        rect(vec2d start_, vec2d end_) : start{start_}, end{end_} {};
        vec2d start;
        vec2d end;
    };
}