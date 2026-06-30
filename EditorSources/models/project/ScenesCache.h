// Copyright (c) 2003-2025, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file ScenesCache.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once
#include "ProjectStateModel.h"
#include "ProjectSubModel.h"
#include "Core/undo/UndoBatch.h"
#include "Engine/display/VisualObject2d.h"
#include "Engine/display/factory/VisualFactory2d.h"

class nsUndoCreateLayout : public nsUndoBatch {
public:
    nsUndoCreateLayout(const nsFilePath &path, nsVisualObject2d *obj);
};


class nsScenesCache final : public nsProjectSubModel {
    friend class nsUndoCreateLayout;
public:
    ~nsScenesCache() override;
    nsVisualObject2d* Get(const std::string &path);
    bool HasFile(const nsFilePath &path) const;

    const std::vector<nsFilePath>& GetFiles() const { return _files; }

    template<typename TVisual>
    TVisual* Create(const char *type) {
        if (auto vis = nsVisualFactory2d::Shared()->CreateByType(type)) {
            if (auto obj = dynamic_cast<TVisual*>(vis)) {
                AddAllocated(obj);
                return obj;
            }
            vis->Destroy();
        }
        return nullptr;
    }

    nsVisualObject2d* Clone(nsVisualObject2d *source);
    nsVisualObject2d* Convert(nsVisualObject2d *source, const char *targetType);
    nsVisualObject2d* ConvertToCustom(nsVisualObject2d *source, const char *customTag);

protected:
    void Reset() override;
    bool Load(const nsFilePath &projectFolder) override;
    bool Save(const nsFilePath &projectFolder) override;

    void Clear();

private:
    std::vector<nsFilePath> _files;
    std::map<std::string, nsVisualObject2d*> _cache;
    std::vector<nsVisualObject2d*> _allocated;

    nsVisualObject2d* ParseSerializedVisual(nsVisualObject2d *source, nsVisualObject2d *target, nsVisualBuilder2d *builder);
    void AddAllocated(nsVisualObject2d *obj);
    static void Destroy(nsVisualObject2d *obj);

};
