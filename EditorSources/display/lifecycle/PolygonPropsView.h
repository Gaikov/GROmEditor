// Copyright (c) 2003-2026, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file PolygonPropsView.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once
#include "view/components/ColorInputUndo.h"
#include "view/library/props/BaseVisualPropsView.h"

class nsPolygonPropsView : public nsBaseVisualPropsView {
public:
    explicit nsPolygonPropsView()
        : nsBaseVisualPropsView("Polygon") {
    }

    bool IsSupport(nsVisualObject2d *target) override;

protected:
    void Draw(nsVisualObject2d *target) override;

    nsColorInputUndo<nsColor> _colorInput = "Color";
};
