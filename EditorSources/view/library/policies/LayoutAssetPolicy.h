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

private:
    nsAppModel *_model = nullptr;
};
