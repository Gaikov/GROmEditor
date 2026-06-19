// Copyright (c) 2003-2026, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file AssetPolicyRegistry.cpp
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#include "AssetPolicyRegistry.h"

#include "policies/ImageAssetPolicy.h"
#include "policies/LayoutAssetPolicy.h"
#include <utility>

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

void nsAssetPolicyRegistry::OnCreated() {
    RegisterPolicy("layout", std::make_shared<nsLayoutAssetPolicy>());
    const auto imagePolicy = std::make_shared<nsImageAssetPolicy>();
    RegisterPolicy("png", imagePolicy);
    RegisterPolicy("jpg", imagePolicy);
}
