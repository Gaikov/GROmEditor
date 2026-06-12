// Copyright (c) 2003-2026, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file AnchorPropsView.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once
#include "view/components/ColorInputUndo.h"
#include "view/components/FloatInputUndo.h"
#include "view/components/RenStateSelectUndo.h"
#include "view/library/props/BaseVisualPropsView.h"

class nsAnchorPropsView : public nsBaseVisualPropsView {
public:
    explicit nsAnchorPropsView()
        : nsBaseVisualPropsView("Anchor") {
    }

    bool IsSupport(nsVisualObject2d *target) override;

protected:
    void Draw(nsVisualObject2d *target) override;

    nsFloatInputUndo<float> _sizeInput = "Size";
    nsColorInputUndo<nsColor> _colorInput = "Color";
    nsRenStateSelectUndo<IRenState*> _renStateInput;
};
