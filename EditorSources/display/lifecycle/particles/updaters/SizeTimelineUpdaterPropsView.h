// Copyright (c) 2003-2026, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file SizeTimelineUpdaterPropsView.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once
#include "UpdaterPropsRegistry.h"
#include "Engine/renderer/particles/updater/size/ParticlesSizeTimelineUpdater.h"
#include "imgui/implot.h"

class nsSizeTimelineUpdaterPropsView final : public nsUpdaterPropsView {
public:
    bool IsSupported(nsParticlesUpdater *object) override;

    void Draw(nsParticlesUpdater *object, nsPropsContext *context) override;

private:
    std::vector<float> _xs;
    std::vector<float> _ys;

    std::vector<nsParticlesSizeTimelineUpdater::Frame> _prevFrames;

    int _selectedPoint = -1;
    int _draggingPoint = -1;

    nsParticlesSizeTimelineUpdater::Frame _prevValue = {};
    ImPlotPoint _addPos;

    void OnDragStarted(const nsParticlesSizeTimelineUpdater *object, int pointIndex);
    void OnDragStopped(const nsParticlesSizeTimelineUpdater *object, int pointIndex) const;

};
