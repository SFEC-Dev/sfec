#include "tui.h"

void tui::render_text(pos start, std::string text) {
    tui::render::write(main->get_matrix(), start, text);
}