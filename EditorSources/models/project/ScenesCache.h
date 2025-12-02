// Copyright (c) 2003-2025, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file ScenesCache.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once
#include "Engine/display/VisualObject2d.h"

class nsScenesCache {
public:
    ~nsScenesCache();
    nsVisualObject2d* Get(const std::string &path);

    bool Load(const nsFilePath &projectFolder);
    void Save(const nsFilePath &projectFolder);

    const std::vector<nsFilePath>& GetFiles() const { return _files; }

private:
    std::vector<nsFilePath> _files;
    std::map<std::string, nsVisualObject2d*> _cache;
};
