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
