// Copyright (c) 2003-2026, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file VisualMaskProps.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once
#include "view/library/props/BaseVisualPropsView.h"

class nsVisualMaskProps : public nsBaseVisualPropsView {
public:
    explicit nsVisualMaskProps()
        : nsBaseVisualPropsView("Masking") {
    }

    bool IsSupport(nsVisualObject2d *target) override;
    void Draw(nsVisualObject2d *target) override;

private:
    std::vector<nsVisualMask*> _masks;
};


