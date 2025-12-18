// Copyright (c) 2003-2025, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file SpawnerPropsView.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once
#include "Engine/renderer/particles/spawner/ParticlesSpawner.h"

class nsSpawnerPropsContext {
public:
    virtual ~nsSpawnerPropsContext() = default;
    virtual void DrawProps(nsParticlesSpawner *spawner) = 0;
};

class nsSpawnerPropsView {
    friend class nsSpawnerPropsRegistry;
protected:
    virtual ~nsSpawnerPropsView() = default;

    virtual bool IsSupported(nsParticlesSpawner *spawner) = 0;
    virtual void Draw(nsParticlesSpawner *spawner, nsSpawnerPropsContext *context) = 0;
};
