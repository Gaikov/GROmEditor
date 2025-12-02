//
// Created by Roman on 11/18/2025.
//

#include "ScenesCache.h"

#include "Engine/display/factory/VisualFactory2d.h"

nsScenesCache::~nsScenesCache() {
    Log::Info("...destroy scenes cache");
    for (const auto &pair : _cache) {
        pair.second->Destroy();
    }
}

nsVisualObject2d * nsScenesCache::Get(const std::string &path) {
    if (!_cache.contains(path)) {
        const auto scene = nsVisualFactory2d::Shared()->Create(path.c_str());
        if (scene) {
            Log::Info("scene cached: %s", path.c_str());
            _cache[path] = scene;
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

bool nsScenesCache::Save(const nsFilePath &projectFolder) {
    return true;
}
