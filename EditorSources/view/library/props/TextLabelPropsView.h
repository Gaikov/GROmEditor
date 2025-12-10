// Copyright (c) 2003-2025, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file TextLabelPropsView.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once
#include "BaseVisualPropsView.h"
#include "Engine/display/align/Align.h"
#include "view/components/BoolInputUndo.h"
#include "view/components/ColorInputUndo.h"
#include "view/components/EnumInputUndo.h"
#include "view/components/FloatInputUndo.h"
#include "view/components/FontSelectUndo.h"
#include "view/components/RenStateSelectUndo.h"
#include "view/components/TextInputUndo.h"

class nsTextLabelPropsView final : public nsBaseVisualPropsView {
public:
    explicit nsTextLabelPropsView()
        : nsBaseVisualPropsView("Label") {
        _alignInput.AddVariant("Left", nsAlign::BEGIN);
        _alignInput.AddVariant("Center", nsAlign::CENTER);
        _alignInput.AddVariant("Right", nsAlign::END);

        _letterSpace.step = 1;
        _letterSpace.fastStep = 10;
    }

    bool IsSupport(nsVisualObject2d *target) override;

protected:
    void Draw(nsVisualObject2d *target) override;

private:
    nsFontSelectUndo<nsFont*>           _fontSelect;
    nsRenStateSelectUndo<IRenState*>    _renStateSelect;
    nsTextInputUndo<nsString>           _textInput = "Text";
    nsColorInputUndo<nsColor>           _colorInput = "Color";
    nsBoolInputUndo<bool>               _drawFrameInput = "Draw Frame";
    nsEnumInputUndo<nsAlign::Type>      _alignInput = "Horizontal Align";
    nsFloatInputUndo<float>             _letterSpace = "Letters Space";
};
