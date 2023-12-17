#include "../tui/utils/icon.h"

#include <filesystem>
#include <vector>

namespace files {
    namespace fs = std::filesystem;

    extern const std::vector<std::u32string> ignored_files;
    
    std::vector<files::fs::path> get_files(const fs::path& dir_patch);
    
    std::vector<std::pair<tui::icons::icon_t, std::u32string>> get_names(const std::vector<fs::path>& from);
}