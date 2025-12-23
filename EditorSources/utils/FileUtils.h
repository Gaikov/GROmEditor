// Copyright (c) 2003-2025, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file FileUtils.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once
#include <string>
#include <vector>

#include "nsLib/FilePath.h"

class nsFileUtils final {
public:
    static bool CheckExtension(const nsFilePath &path, const std::vector<std::string> &extensions);
};
