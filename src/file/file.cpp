#include "file.h"

#include <algorithm>

bool default_comparator(const files::fs::directory_entry& first, const files::fs::directory_entry& second) {
    if (first.is_directory()) {
        if (second.is_directory()) {
            return (first.path().filename() < second.path().filename());
        }
        return true;
    } else {
        if (second.is_directory())
            return false;
    }

    return (first.path().filename() < second.path().filename());
}

void files::default_sort(std::vector<fs::directory_entry>& dir_content) {
    std::sort(dir_content.begin(), dir_content.end(), default_comparator);
}