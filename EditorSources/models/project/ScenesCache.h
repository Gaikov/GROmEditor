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
    bool Save(const nsFilePath &projectFolder) const;

    const std::vector<nsFilePath>& GetFiles() const { return _files; }

    template<typename TVisual>
    TVisual* Create() {
        auto obj = new TVisual();
        AddAllocated(obj);
        return obj;
    }

private:
    std::vector<nsFilePath> _files;
    std::map<std::string, nsVisualObject2d*> _cache;
    std::vector<nsVisualObject2d*> _allocated;

    void AddAllocated(nsVisualObject2d *obj);
    static void Destroy(nsVisualObject2d *obj);

};
