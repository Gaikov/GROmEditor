// Copyright (c) 2003-2025, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file SpritePropsView.h
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#pragma once
#include "BaseVisualPropsView.h"
#include "view/components/ColorInputUndo.h"
#include "view/components/TextureSelectUndo.h"

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
};
