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
#include "Engine/display/default/DefaultVisualObject.h"
#include "Engine/display/factory/VisualFactory2d.h"
#include "models/AppModel.h"
#include "nsLib/locator/ServiceLocator.h"
#include "utils/FileUtils.h"
#include "view/alerts/AlertPopup.h"

nsUndoCreateLayout::nsUndoCreateLayout(const nsFilePath &path, nsVisualObject2d *obj) {
    auto &project = Locate<nsAppModel>()->project;
    auto &scenes = project.scenes;
    scenes._allocated.push_back(obj);

    const auto writer = std::make_shared<nsStringWriter>();
    std::string buffer = "//created by GROm Editor\n";
    nsScriptSaver saver(writer);
    if (nsVisualFactory2d::Shared()->Serialize(saver, obj)) {
        buffer += writer->GetBuffer();
    }

    Add(new nsUndoFileCreate(path, buffer.c_str()));
    Add(new nsUndoVectorAdd(scenes._files, path));
    Add(new nsUndoMapInsert<std::string, nsVisualObject2d *>(scenes._cache, path.AsChar(), obj));
    Add(new nsUndoVarChange<nsFilePathVar, std::string>(project.user.currentScene, path.AsChar()));
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

bool nsScenesCache::HasFile(const nsFilePath &path) const {
    if (path.IsEmpty()) {
        return false;
    }

    for (const auto &file : _files) {
        if (file == path) {
            return true;
        }
    }

    return false;
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

nsVisualObject2d *nsScenesCache::Convert(nsVisualObject2d *source, const char *targetType) {
    const auto factory = nsVisualFactory2d::Shared();
    const auto targetBuilder = factory->GetBuilder(targetType);
    if (!targetBuilder) {
        return nullptr;
    }

    auto converted = factory->CreateByType(targetType);
    return ParseSerializedVisual(source, converted, targetBuilder);
}

nsVisualObject2d *nsScenesCache::ConvertToCustom(nsVisualObject2d *source, const char *customTag) {
    const auto factory = nsVisualFactory2d::Shared();
    const auto defaultBuilder = factory->GetDefaultBuilder();
    if (!defaultBuilder) {
        return nullptr;
    }

    const auto parsed = ParseSerializedVisual(source, new nsDefaultVisualObject(), defaultBuilder);
    auto converted = dynamic_cast<nsDefaultVisualObject *>(parsed);
    if (converted) {
        converted->SetTagName(customTag);
    }

    return converted;
}

nsVisualObject2d *nsScenesCache::ParseSerializedVisual(nsVisualObject2d *source, nsVisualObject2d *target,
                                                       nsVisualBuilder2d *builder) {
    if (!source || !target || !builder) {
        if (target) {
            target->Destroy();
        }
        return nullptr;
    }

    const auto factory = nsVisualFactory2d::Shared();
    const auto writer = std::make_shared<nsStringWriter>();
    nsScriptSaver saver(writer);
    if (!factory->Serialize(saver, source)) {
        target->Destroy();
        return nullptr;
    }

    auto buffer = writer->GetBuffer();
    if (const auto ss = ps_begin(buffer.data())) {
        if (ps_block_begin(ss, nullptr) && builder->Parse(ss, target, factory)) {
            ps_end(ss);
            AddAllocated(target);
            return target;
        }
        ps_end(ss);
    }

    target->Destroy();
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
        const auto obj = pair.second;
        const auto parent = obj->GetParent();
        if (parent) {
            parent->RemoveChild(obj);
        }

        if (!vf->Serialize(pair.first.c_str(), pair.second)) {
            result = false;
        }

        if (parent) {
            parent->AddChild(obj);
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
