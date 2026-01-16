// Copyright (c) 2003-2025, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file VisualPropsView.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once
#include "BaseVisualPropsView.h"
#include "view/components/AngleInputUndo.h"
#include "view/components/BoolInputUndo.h"
#include "view/components/TextInputUndo.h"
#include "view/components/Vec2InputUndo.h"

class nsVisualPropsView : public nsBaseVisualPropsView {
public:
    explicit nsVisualPropsView()
        : nsBaseVisualPropsView("Common") {
    }
    void Draw(nsVisualObject2d *target) override;
    bool IsSupport(nsVisualObject2d *target) override {
        return true;
    };

    bool DrawContextMenu(nsVisualObject2d *target, bool hasPrevItems) override;

private:
    nsTextInputUndo<std::string> _idInput = "ID";
    nsTextInputUndo<std::string> _bindingInput = "Binding Id";
    nsBoolInputUndo<nsProperty<bool>> _visibleInput = "Visible";
    nsVec2InputUndo<nsProperty<nsVec2>> _posInput = "Position";
    nsVec2InputUndo<nsProperty<nsVec2>> _scaleInput = "Scale";
    nsAngleInputUndo<nsProperty<float>> _angleInput = "Rotation";
};
