// Copyright (c) 2003-2026, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file ColorTimelineUpdaterPropsView.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once
#include "UpdaterPropsRegistry.h"
#include "Engine/renderer/particles/updater/ParticlesColorTimelineUpdater.h"
#include "view/components/ColorInputUndo.h"

class nsColorTimelineUpdaterPropsView final : public nsUpdaterPropsView {
public:
    bool IsSupported(nsParticlesUpdater *object) override;

    void Draw(nsParticlesUpdater *object, nsPropsContext *context) override;

private:
    std::vector<nsParticlesColorTimelineUpdater::Frame> _prevFrames;

    int _selectedPoint = -1;
    int _draggingPoint = -1;

    nsParticlesColorTimelineUpdater::Frame _prevValue = {};

    nsColorInputUndo<nsColor> _colorInput = "Color";

    void OnDragStarted(const nsParticlesColorTimelineUpdater *object, int pointIndex);
    void OnDragStopped(const nsParticlesColorTimelineUpdater *object, int pointIndex) const;
};
