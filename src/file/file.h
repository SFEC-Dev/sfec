#include "../tui/utils/icon.h"

#include <filesystem>
#include <vector>

namespace files {
    namespace fs = std::filesystem;

    extern const std::vector<std::u32string> ignored_files;
    
    std::vector<files::fs::directory_entry> get_files(const fs::path& dir_patch);
    
    std::vector<std::pair<tui::icons::icon_t, std::u32string>> get_names(const std::vector<fs::directory_entry>& from);

    std::vector<std::u32string> read_file(fs::path path);
}
