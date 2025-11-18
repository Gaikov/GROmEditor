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
