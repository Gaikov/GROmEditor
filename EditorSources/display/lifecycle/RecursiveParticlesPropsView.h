// Copyright (c) 2003-2026, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file RecursiveParticlesPropsView.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once

#include "Engine/display/particles/VisualParticles.h"
#include "view/library/props/BaseVisualPropsView.h"

class nsRecursiveParticlesPropsView final : public nsBaseVisualPropsView {
public:
    nsRecursiveParticlesPropsView();
    bool IsSupport(nsVisualObject2d *target) override;

protected:
    void Draw(nsVisualObject2d *target) override;

    std::vector<nsVisualParticles*> _particles;
};
