//
// Created by Roman on 12/24/2025.
//

#include "ParticlesCache.h"
#include "Core/undo/file/UndoFileCreate.h"

nsParticlesCache::~nsParticlesCache() {
    Reset();
}

nsParticlesBehaviour * nsParticlesCache::LoadParticles(const char *fileName) {
    const std::string path = fileName;

    if (!_cache.contains(path)) {
        if (const auto res = _factory.LoadBehaviour(path.c_str())) {
            _cache[path] = res;
        } else {
            return nullptr;
        }
    }

    return _cache[path];
}

const char * nsParticlesCache::GetParticlesPath(nsParticlesBehaviour *particles) {
    for (const auto &item: _cache) {
        if (item.second == particles) {
            return item.first.c_str();
        }
    }
    return "";
}

nsParticlesBehaviour * nsParticlesCache::CreateDefault() {
    const auto res = _factory.LoadBehaviour("default/editor/particles.ggps");
    assert(res);
    return res;
}

void nsParticlesCache::Reset() {
    for (const auto &item: _cache) {
        delete item.second;
    }
    _cache.clear();
}

bool nsParticlesCache::Load(const nsFilePath &folder) {
    return true;
}

bool nsParticlesCache::Save(const nsFilePath &folder) {
    for (const auto &item: _cache) {
        if (!_factory.SaveBehaviour(item.second, item.first.c_str())) {
            return false;
        }
    }

    return true;
}

nsUndoCreateParticles::nsUndoCreateParticles(nsParticlesCache &cache, const std::string &path) {
    _created = cache.CreateDefault();

    Add(new nsUndoFileCreate(path.c_str(), ""));
    Add(new nsUndoMapInsert(cache._cache, path, _created));
}
