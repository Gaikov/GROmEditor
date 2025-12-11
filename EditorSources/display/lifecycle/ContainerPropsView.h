// Copyright (c) 2003-2025, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file ContainerPropsView.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once
#include "view/components/BoolInputUndo.h"
#include "view/library/props/BaseVisualPropsView.h"

class nsContainerPropsView : public nsBaseVisualPropsView {
public:
    explicit nsContainerPropsView()
        : nsBaseVisualPropsView("container") {
    }
    bool IsSupport(nsVisualObject2d *target) override;

    bool DrawContextMenu(nsVisualObject2d *target, bool hasPrevItems) override;

protected:
    void Draw(nsVisualObject2d *target) override;

private:
    nsBoolInputUndo<bool> _interactiveInput = "Interactive Children";
};
