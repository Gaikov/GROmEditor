// Copyright (c) 2003-2026, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file UpdaterPropsRegistry.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once
#include "display/lifecycle/particles/ParticlesBasePropsView.h"
#include "Engine/renderer/particles/updater/ParticlesUpdater.h"

class nsUpdaterPropsView : public nsBasePropsView<nsParticlesUpdater> {

};

class nsUpdaterPropsRegistry final : public nsBasePropsRegistry<nsParticlesUpdater> {
public:
    nsUpdaterPropsRegistry();
};
