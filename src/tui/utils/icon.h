#pragma once
#include "../render/color.h"

#include <filesystem>

namespace tui::icons {
    using icon_t = std::pair<char32_t, tui::Color>;
    icon_t get_icon(std::filesystem::path);
}