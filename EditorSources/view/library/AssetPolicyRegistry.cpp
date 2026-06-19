// Copyright (c) 2003-2026, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file AssetPolicyRegistry.cpp
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#include "AssetPolicyRegistry.h"

#include "Core/undo/UndoBatch.h"
#include "Core/undo/UndoService.h"
#include "Core/undo/UndoVarChange.h"
#include "Engine/display/VisualObject2d.h"
#include "models/AppModel.h"
#include "nsLib/locator/ServiceLocator.h"
#include "nsLib/log.h"
#include <string>
#include <utility>

class nsLayoutAssetPolicy final : public IAssetPolicy {
public:
    nsLayoutAssetPolicy() {
        _model = Locate<nsAppModel>();
    }

    void OnDoubleClick(const nsFilePath &path) override {
        Log::Info("Selected: %s", path.AsChar());

        auto &user = _model->project.user;
        const std::string value = path.AsChar();
        const auto batch = new nsUndoBatch();

        batch->Add(new nsUndoVarChange(user.selectedObject,
                                       static_cast<nsVisualObject2d *>(nullptr)));
        batch->Add(new nsUndoVarChange(user.currentScene, value));

        nsUndoService::Shared()->Push(batch);
    }

    bool IsSelected(const nsFilePath &path) const override {
        return _model->project.user.currentScene == path.AsChar();
    }

private:
    nsAppModel *_model = nullptr;
};

class nsImageAssetPolicy final : public IAssetPolicy {
public:
    void OnDoubleClick(const nsFilePath &path) override {
    }
};

void nsAssetPolicyRegistry::RegisterPolicy(const char *extension, std::shared_ptr<IAssetPolicy> policy) {
    if (!extension || !policy) {
        return;
    }

    Entry entry;
    entry.extension = extension[0] == '.' ? extension + 1 : extension;
    entry.policy = std::move(policy);
    _entries.push_back(entry);
}

IAssetPolicy *nsAssetPolicyRegistry::FindPolicy(const nsFilePath &path) const {
    for (const auto &entry : _entries) {
        if (path.CheckExtension(entry.extension)) {
            return entry.policy.get();
        }
    }
    return nullptr;
}

bool nsAssetPolicyRegistry::HasPolicy(const nsFilePath &path) const {
    return FindPolicy(path) != nullptr;
}

bool nsAssetPolicyRegistry::OnInit() {
    nsSubSystem::OnInit();

    RegisterPolicy("layout", std::make_shared<nsLayoutAssetPolicy>());
    const auto imagePolicy = std::make_shared<nsImageAssetPolicy>();
    RegisterPolicy("png", imagePolicy);
    RegisterPolicy("jpg", imagePolicy);

    return true;
}

void nsAssetPolicyRegistry::OnRelease() {
    _entries.clear();
    nsSubSystem::OnRelease();
}
