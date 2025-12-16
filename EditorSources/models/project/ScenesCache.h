// Copyright (c) 2003-2025, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file ScenesCache.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once
#include "ProjectSubModel.h"
#include "Engine/display/VisualObject2d.h"

class nsScenesCache final : public nsProjectSubModel {
public:
    ~nsScenesCache() override;
    nsVisualObject2d* Get(const std::string &path);

    const std::vector<nsFilePath>& GetFiles() const { return _files; }

    template<typename TVisual>
    TVisual* Create() {
        auto obj = new TVisual();
        AddAllocated(obj);
        return obj;
    }
protected:
    void Reset() override;
    bool Load(const nsFilePath &projectFolder) override;
    bool Save(const nsFilePath &projectFolder) override;

    void Clear();

private:
    std::vector<nsFilePath> _files;
    std::map<std::string, nsVisualObject2d*> _cache;
    std::vector<nsVisualObject2d*> _allocated;

    void AddAllocated(nsVisualObject2d *obj);
    static void Destroy(nsVisualObject2d *obj);

};
