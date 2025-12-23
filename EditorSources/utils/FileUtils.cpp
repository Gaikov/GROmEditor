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

nsFilePath nsFileUtils::EnsureExtension(const nsFilePath &path, const std::vector<std::string> &extensions) {
    if (CheckExtension(path, extensions)) {
        return path;
    }
    auto name = path.GetNameOnly();
    name += ".";
    name += extensions[0].c_str();

    return path.GetParent().ResolvePath(name);
}
