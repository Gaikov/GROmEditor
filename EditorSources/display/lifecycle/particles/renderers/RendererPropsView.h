// Copyright (c) 2003-2026, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file RendererPropsView.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once
#include "display/lifecycle/particles/ParticlesBasePropsView.h"
#include "Engine/renderer/particles/ParticlesBehaviour.h"
#include "Engine/renderer/particles/renderer/ParticlesRenderer.h"
#include "Engine/renderer/particles/renderer/ParticlesRendererFactory.h"

class nsRendererPropsView : public nsBasePropsView<nsParticlesRenderer> {
};

class nsRendererPropsRegistry final : protected nsBasePropsRegistry<nsParticlesRenderer> {
public:
    nsRendererPropsRegistry();
    void Draw(nsParticlesBehaviour *b);

private:
    nsParticlesRendererFactory _factory;
    std::vector<std::string> _names;
};
