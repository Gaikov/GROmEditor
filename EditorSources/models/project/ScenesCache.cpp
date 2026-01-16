//
// Created by Roman on 11/18/2025.
//

#include "ScenesCache.h"

#include "Core/buffer/StringWriter.h"
#include "Core/undo/UndoMapInsert.h"
#include "Core/undo/UndoVarChange.h"
#include "Core/undo/UndoVectorAdd.h"
#include "Core/undo/file/UndoFileCreate.h"
#include "Engine/display/container/VisualContainer2d.h"
#include "Engine/display/factory/VisualFactory2d.h"
#include "models/AppModel.h"
#include "nsLib/locator/ServiceLocator.h"
#include "utils/FileUtils.h"
#include "view/alerts/AlertPopup.h"

nsUndoCreateLayout::nsUndoCreateLayout(const nsFilePath &path, nsVisualObject2d *obj) {
    auto &project = Locate<nsAppModel>()->project;
    auto &scenes = project.scenes;
    scenes._allocated.push_back(obj);

    Add(new nsUndoFileCreate(path, "//created layout"));
    Add(new nsUndoVectorAdd(scenes._files, path));
    Add(new nsUndoMapInsert<std::string, nsVisualObject2d *>(scenes._cache, path.AsChar(), obj));
    Add(new nsUndoVarChange<nsStringVar, std::string>(project.user.currentScene, path.AsChar()));
    Add(new nsUndoVarChange(project.user.selectedObject, obj));
}

nsScenesCache::~nsScenesCache() {
    Clear();
}

nsVisualObject2d *nsScenesCache::Get(const std::string &path) {
    if (path.empty()) {
        return nullptr;
    }

    if (!_cache.contains(path)) {
        const auto scene = nsVisualFactory2d::Shared()->Create(path.c_str());
        if (scene) {
            Log::Info("scene cached: %s", path.c_str());
            _cache[path] = scene;
            AddAllocated(scene);
        }
        return scene;
    }
    return _cache[path];
}

nsVisualObject2d * nsScenesCache::Clone(nsVisualObject2d *source) {
    const auto writer = std::make_shared<nsStringWriter>();
    nsScriptSaver saver(writer);
    if (nsVisualFactory2d::Shared()->Serialize(saver, source)) {
        auto buffer = writer->GetBuffer();

        if (const auto ss = ps_begin(buffer.data())) {
            if (ps_block_begin(ss, nullptr)) {
                const auto object = nsVisualFactory2d::Shared()->Create(ss);
                ps_end(ss);
                AddAllocated(object);
                return object;
            }
        }
    }

    return nullptr;
}

void nsScenesCache::Reset() {
    Clear();
}

void nsScenesCache::Clear() {
    Log::Info("...destroy scenes cache");
    for (const auto obj: _allocated) {
        Destroy(obj);
    }
    _allocated.clear();
    _cache.clear();
    _files.clear();
}

bool nsScenesCache::Load(const nsFilePath &projectFolder) {
    Log::Info("Loading scenes cache from folder: %s", projectFolder.AsChar());
    _files.clear();

    nsFilePath::tList list;
    projectFolder.ListingRecursive(list);

    const std::vector<std::string> extensions = {"layout"};

    for (auto item: list) {
        if (nsFileUtils::CheckExtension(item, extensions)) {
            Log::Info("Found layout file: %s", item.AsChar());
            _files.push_back(item);
        }
    }
    return true;
}

bool nsScenesCache::Save(const nsFilePath &projectFolder) {
    Log::Info("Saving scenes cache to folder: %s", projectFolder.AsChar());
    bool result = true;
    const auto vf = nsVisualFactory2d::Shared();

    for (const auto &pair: _cache) {
        if (!vf->Serialize(pair.first.c_str(), pair.second)) {
            result = false;
        }
    }

    return result;
}

void nsScenesCache::AddAllocated(nsVisualObject2d *obj) {
    assert(obj);
    _allocated.push_back(obj);

    if (const auto container = dynamic_cast<nsVisualContainer2d *>(obj)) {
        for (const auto child: container->GetChildren()) {
            AddAllocated(child);
        }
    }
}

void nsScenesCache::Destroy(nsVisualObject2d *obj) {
    if (auto const container = dynamic_cast<nsVisualContainer2d *>(obj)) {
        container->RemoveChildren();
    }
    obj->Destroy();
}
