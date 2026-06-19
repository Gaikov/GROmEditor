// Copyright (c) 2003-2026, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file LayoutAssetPolicy.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once

#include "view/library/AssetPolicyRegistry.h"

class nsAppModel;

class nsLayoutAssetPolicy final : public IAssetPolicy {
public:
    nsLayoutAssetPolicy();

    void OnDoubleClick(const nsFilePath &path) override;
    bool IsSelected(const nsFilePath &path) const override;
    bool CanCreateVisual() const override { return true; }
    nsVisualObject2d *CreateVisual(const nsFilePath &path) override;

private:
    nsAppModel *_model = nullptr;

    bool ContainsSceneReference(nsVisualObject2d *visual, const nsFilePath &scenePath) const;
};
