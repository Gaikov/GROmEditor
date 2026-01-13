// Copyright (c) 2003-2026, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file SizeTimelineUpdaterPropsView.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once
#include "UpdaterPropsRegistry.h"

class nsSizeTimelineUpdaterPropsView final : public nsUpdaterPropsView {
public:
    bool IsSupported(nsParticlesUpdater *object) override;
    void Draw(nsParticlesUpdater *object, nsPropsContext *context) override;

private:
    std::vector<float> xs;
    std::vector<float> ys;
};
