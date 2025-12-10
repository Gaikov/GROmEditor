// Copyright (c) 2003-2025, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file TextLabelPropsView.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once
#include "BaseVisualPropsView.h"
#include "view/components/FontSelectUndo.h"

class nsTextLabelPropsView final : public nsBaseVisualPropsView {
public:
    explicit nsTextLabelPropsView()
        : nsBaseVisualPropsView("Label") {
    }
    bool IsSupport(nsVisualObject2d *target) override;

protected:
    void Draw(nsVisualObject2d *target) override;

private:
    nsFontSelectUndo<nsFont*>   _fontSelect;
};
