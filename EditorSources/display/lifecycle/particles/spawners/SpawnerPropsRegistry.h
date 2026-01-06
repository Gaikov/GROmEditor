// Copyright (c) 2003-2025, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file SpawnerPropsRegistry.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once
#include "display/lifecycle/particles/ParticlesBasePropsView.h"
#include "Engine/renderer/particles/spawner/ParticlesSpawner.h"

class nsSpawnerPropsView : public nsBasePropsView<nsParticlesSpawner> {
};

class nsSpawnerPropsRegistry : public nsBasePropsRegistry<nsParticlesSpawner> {
public:
    nsSpawnerPropsRegistry();
};
