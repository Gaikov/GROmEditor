// Copyright (c) 2003-2025, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file ParticlesAssetPropsView.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once
#include "spawners/SpawnerPropsRegistry.h"
#include "view/components/FloatInputUndo.h"
#include "view/components/IntInputUndo.h"
#include "view/library/props/BaseVisualPropsView.h"

class nsParticlesAssetPropsView : public nsBaseVisualPropsView {
public:
    explicit nsParticlesAssetPropsView()
        : nsBaseVisualPropsView("Particles Asset") {
    }
    bool IsSupport(nsVisualObject2d *target) override;

protected:
    void Draw(nsVisualObject2d *target) override;

    nsSpawnerPropsRegistry _spawnerProps;

    nsIntInputUndo<int> _particlesPerSec = "Particles Per Sec";
    nsFloatInputUndo<float> _blastTime = "Blast Time";
};
