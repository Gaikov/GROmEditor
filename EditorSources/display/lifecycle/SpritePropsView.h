// Copyright (c) 2003-2025, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file SpritePropsView.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once
#include "view/components/BoolInputUndo.h"
#include "view/library/props/BaseVisualPropsView.h"
#include "view/components/ColorInputUndo.h"
#include "view/components/RenStateSelectUndo.h"
#include "view/components/TextureSelectUndo.h"
#include "view/components/Vec2InputUndo.h"

class nsSpritePropsView : public nsBaseVisualPropsView {
public:
    explicit nsSpritePropsView()
        : nsBaseVisualPropsView("Sprite") {
    }

    bool IsSupport(nsVisualObject2d *target) override;

protected:
    void Draw(nsVisualObject2d *target) override;

    nsColorInputUndo<nsColor> _colorInput = "Color";
    nsTextureSelectUndo<ITexture*> _textureInput = "Texture";
    nsRenStateSelectUndo<IRenState*> _shaderInput;
    nsVec2InputUndo<nsVec2> _uv1Input = "UV1";
    nsVec2InputUndo<nsVec2> _uv2Input = "UV2";
    nsVec2InputUndo<nsVec2> _pivotInput = "Pivot";
    nsVec2InputUndo<nsVec2> _sizeInput = "Rect Size";
    nsBoolInputUndo<bool> _frameInput = "Draw Frame";
};
