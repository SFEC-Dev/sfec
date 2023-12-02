#include "tui.h"

namespace selector {
std::string left() {
    return "[%dD";
}
std::string right() {
    return "[%dC";
}
std::string up() {
    return "[%dA";
}
std::string down() {
    return "[%dB";
}
} // namespace selector
