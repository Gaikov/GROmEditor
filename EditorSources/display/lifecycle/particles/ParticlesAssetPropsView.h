// Copyright (c) 2003-2025, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file ParticlesAssetPropsView.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once
#include "view/library/props/BaseVisualPropsView.h"

class nsParticlesAssetPropsView : public nsBaseVisualPropsView {
public:
    explicit nsParticlesAssetPropsView()
        : nsBaseVisualPropsView("Particles Asset") {
    }
    bool IsSupport(nsVisualObject2d *target) override;

protected:
    void Draw(nsVisualObject2d *target) override;
};
