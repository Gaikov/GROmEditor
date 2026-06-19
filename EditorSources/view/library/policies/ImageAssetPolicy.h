// Copyright (c) 2003-2026, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file ImageAssetPolicy.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once

#include "view/library/AssetPolicyRegistry.h"

class nsImageAssetPolicy final : public IAssetPolicy {
public:
    void OnDoubleClick(const nsFilePath &path) override;
};
