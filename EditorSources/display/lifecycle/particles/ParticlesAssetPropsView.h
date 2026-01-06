// Copyright (c) 2003-2025, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file ParticlesAssetPropsView.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once
#include "renderers/RendererPropsView.h"
#include "spawners/SpawnerPropsRegistry.h"
#include "updaters/UpdaterPropsRegistry.h"
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
    nsUpdaterPropsRegistry _updaterProps;
    nsRendererPropsRegistry _rendererProps;

    nsIntInputUndo<int> _particlesPerSec = "Particles Per Sec";
    nsFloatInputUndo<float> _blastTime = "Blast Time";
};
