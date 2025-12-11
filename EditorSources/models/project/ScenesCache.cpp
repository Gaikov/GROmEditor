//
// Created by Roman on 11/18/2025.
//

#include "ScenesCache.h"

#include "Engine/display/container/VisualContainer2d.h"
#include "Engine/display/factory/VisualFactory2d.h"

nsScenesCache::~nsScenesCache() {
    Log::Info("...destroy scenes cache");
    for (const auto obj : _allocated) {
        Destroy(obj);
    }
}

nsVisualObject2d * nsScenesCache::Get(const std::string &path) {
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

bool nsScenesCache::Load(const nsFilePath &projectFolder) {
    Log::Info("Loading scenes cache from folder: %s", projectFolder.AsChar());
    _files.clear();

    nsFilePath::tList list;
    projectFolder.ListingRecursive(list);

    for (auto item: list) {
        auto ext = item.GetExtension();
        ext.ToLower();
        if (ext == "layout") {
            Log::Info("Found layout file: %s", item.AsChar());
            _files.push_back(item);
        }
    }
    return true;
}

bool nsScenesCache::Save(const nsFilePath &projectFolder) const {
    Log::Info("Saving scenes cache to folder: %s", projectFolder.AsChar());
    bool result = true;
    const auto vf = nsVisualFactory2d::Shared();

    for (const auto &pair : _cache) {
        if (!vf->Serialize(pair.first.c_str(), pair.second)) {
            result = false;
        }
    }

    return result;
}

void nsScenesCache::AddAllocated(nsVisualObject2d *obj) {
    _allocated.push_back(obj);

    if (const auto container = dynamic_cast<nsVisualContainer2d*>(obj)) {
        for (const auto child : container->GetChildren()) {
            AddAllocated(child);
        }
    }
}

void nsScenesCache::Destroy(nsVisualObject2d *obj) {
    if (auto const container = dynamic_cast<nsVisualContainer2d*>(obj)) {
        container->RemoveChildren();
    }
    obj->Destroy();
}
