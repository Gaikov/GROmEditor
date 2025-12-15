// Copyright (c) 2003-2025, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file ParticlesPropsView.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once
#include "Engine/display/particles/VisualParticles.h"
#include "view/components/BoolInputUndo.h"
#include "view/components/EnumInputUndo.h"
#include "view/library/props/BaseVisualPropsView.h"

class nsParticlesPropsView : public nsBaseVisualPropsView {
public:
    explicit nsParticlesPropsView()
        : nsBaseVisualPropsView("Particles") {
        _spaceInput.AddVariant("local", nsVisualParticles::LOCAL);
        _spaceInput.AddVariant("global", nsVisualParticles::GLOBAL);
        _spaceInput.AddVariant("parent", nsVisualParticles::PARENT);
    }
    bool IsSupport(nsVisualObject2d *target) override;

protected:
    void Draw(nsVisualObject2d *target) override;

    nsBoolInputUndo<bool> _spawnInput = "Spawn";
    nsEnumInputUndo<nsVisualParticles::Space, nsProperty<nsVisualParticles::Space>> _spaceInput = "Space";
};
