// Copyright (c) 2003-2025, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file LayoutRefPropsView.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once
#include "BaseVisualPropsView.h"
#include "view/components/LayoutRefSelectUndo.h"

class nsLayoutRefPropsView final : public nsBaseVisualPropsView {
public:
    explicit nsLayoutRefPropsView()
        : nsBaseVisualPropsView("Layout reference") {
    }
    bool IsSupport(nsVisualObject2d *target) override;

protected:
    void Draw(nsVisualObject2d *target) override;

private:
    nsLayoutRefSelectUndo _layoutSelect;
};

