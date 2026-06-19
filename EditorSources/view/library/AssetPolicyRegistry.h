// Copyright (c) 2003-2026, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file AssetPolicyRegistry.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once

#include "nsLib/FilePath.h"
#include "nsLib/locator/Locatable.h"
#include <memory>
#include <vector>

class nsVisualObject2d;

class IAssetPolicy {
public:
    virtual ~IAssetPolicy() = default;

    virtual void OnDoubleClick(const nsFilePath &path) = 0;
    virtual bool IsSelected(const nsFilePath &) const { return false; }
    virtual bool CanCreateVisual() const { return false; }
    virtual nsVisualObject2d *CreateVisual(const nsFilePath &) { return nullptr; }
};

class nsAssetPolicyRegistry final : public nsLocatable {
public:
    void RegisterPolicy(const char *extension, std::shared_ptr<IAssetPolicy> policy);
    IAssetPolicy *FindPolicy(const nsFilePath &path) const;
    bool HasPolicy(const nsFilePath &path) const;

protected:
    void OnCreated() override;

private:
    struct Entry {
        nsString extension;
        std::shared_ptr<IAssetPolicy> policy;
    };

    std::vector<Entry> _entries;
};
