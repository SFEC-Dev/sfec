#include "tui.h"

tui::io& tui::get_io() {
    static io global_io{};
    return global_io;
}