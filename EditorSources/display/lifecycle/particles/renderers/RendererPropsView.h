// Copyright (c) 2003-2026, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file RendererPropsView.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once
#include "Engine/renderer/particles/ParticlesBehaviour.h"
#include "Engine/renderer/particles/renderer/ParticlesRenderer.h"
#include "Engine/renderer/particles/renderer/ParticlesRendererFactory.h"

class nsRendererPropsView {
public:
    virtual ~nsRendererPropsView() = default;
    virtual bool IsSupported(nsParticlesRenderer *r) = 0;
    virtual void Draw(nsParticlesRenderer *r) = 0;
};

class nsRendererPropsRegistry final {
public:
    nsRendererPropsRegistry();
    void DrawProps(nsParticlesBehaviour *b);

private:
    std::vector<std::unique_ptr<nsRendererPropsView>> _props;
    nsParticlesRendererFactory _factory;
    std::vector<std::string> _names;
};
