#include <filesystem>
#include <vector>

namespace files {
    namespace fs = std::filesystem;
    
    void default_sort(std::vector<fs::directory_entry>& dir_content);
}