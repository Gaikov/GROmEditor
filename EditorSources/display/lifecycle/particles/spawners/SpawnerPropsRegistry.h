// Copyright (c) 2003-2025, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file SpawnerPropsRegistry.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once
#include <memory>

#include "SpawnerPropsView.h"

class nsSpawnerPropsRegistry : public nsSpawnerPropsContext {
public:
    nsSpawnerPropsRegistry();
    void DrawProps(nsParticlesSpawner *spawner) override;

private:
    std::vector<std::shared_ptr<nsSpawnerPropsView>> _views;
};
