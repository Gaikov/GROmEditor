//
// Created by Roman on 12/23/2025.
//

#include "FileUtils.h"

bool nsFileUtils::CheckExtension(const nsFilePath &path, const std::vector<std::string> &extensions) {
    if (extensions.empty()) {
        return true;
    }

    auto ext = path.GetExtension();
    ext.ToLower();
    for (auto &i : extensions) {
        if (ext == i.c_str()) {
            return true;
        }
    }
    return false;
}

void nsFileUtils::EnsureExtension(nsFilePath &path, const std::vector<std::string> &extensions) {
    if (CheckExtension(path, extensions)) {
        return;
    }
    auto name = path.GetNameOnly();
    name += ".";
    name += extensions[0].c_str();

    path = path.GetParent().ResolvePath(name);
}
