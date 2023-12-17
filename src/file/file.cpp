#include "file.h"

#include <algorithm>

const std::vector<std::u32string> files::ignored_files {
    U"lost+found"
};

bool default_comparator(const files::fs::path& first, const files::fs::path& second) {
    if (is_directory(first)) {
        if (is_directory(second)) {
            return (first.filename() < second.filename());
        }
        return true;
    } else {
        if (is_directory(second))
            return false;
    }

    return (first.filename() < second.filename());
}

std::vector<files::fs::path> files::get_files(const fs::path& dir_patch) {
    std::vector<files::fs::path> result;
    for (const auto& dir_entry : files::fs::directory_iterator{dir_patch, files::fs::directory_options::skip_permission_denied}) {
        if (std::find(files::ignored_files.cbegin(), files::ignored_files.cend(), dir_entry.path().filename().u32string()) != files::ignored_files.cend())
            continue;

        result.emplace_back(dir_entry);
    }

    std::sort(result.begin(), result.end(), default_comparator);
    return result;
}

std::vector<std::pair<tui::icons::icon_t, std::u32string>> files::get_names(const std::vector<files::fs::path>& from) {
    std::vector<std::pair<tui::icons::icon_t, std::u32string>> result;
    result.resize(from.size());

    std::transform(from.cbegin(), from.cend(), result.begin(), [](const files::fs::path& entry){ 
        return std::pair<tui::icons::icon_t, std::u32string>(tui::icons::get_icon(entry), entry.filename().u32string());
    });

    return result;
};