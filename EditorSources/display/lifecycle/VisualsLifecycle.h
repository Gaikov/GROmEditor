// Copyright (c) 2003-2025, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file VisualsRegistry.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once
#include "nsLib/SubSystem.h"
#include "view/library/props/BaseVisualPropsView.h"

class nsVisualsLifecycle : public nsSubSystem<nsVisualsLifecycle> {
public:
    nsVisualsLifecycle();

    void DrawProps(nsVisualObject2d *target) const;
    void DrawContextMenu(nsVisualObject2d *target) const;

private:
    std::vector<nsBaseVisualPropsView::sp_t> _policies;
};
