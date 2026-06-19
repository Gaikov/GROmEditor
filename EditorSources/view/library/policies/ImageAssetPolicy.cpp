// Copyright (c) 2003-2026, Roman Gaikov. All rights reserved.
//--------------------------------------------------------------------------------------------------
// file ImageAssetPolicy.cpp
// author Roman Gaikov
//--------------------------------------------------------------------------------------------------
#include "ImageAssetPolicy.h"

#include "Engine/RenManager.h"
#include "Engine/display/VisualType.h"
#include "Engine/display/sprite/Sprite.h"
#include "models/AppModel.h"
#include "nsLib/locator/ServiceLocator.h"

void nsImageAssetPolicy::OnDoubleClick(const nsFilePath &) {
}

nsVisualObject2d *nsImageAssetPolicy::CreateVisual(const nsFilePath &path) {
    const auto model = Locate<nsAppModel>();
    auto *sprite = model->project.scenes.Create<nsSprite>(nsVisualType::SPRITE);
    if (!sprite) {
        return nullptr;
    }

    auto *device = nsRenDevice::Shared()->Device();
    sprite->id = path.GetNameOnly().AsChar();
    sprite->desc.premultiplyAlpha = true;
    sprite->desc.tex = device->TextureLoad(path.AsChar(), false, TF_RGBA, TLF_PREMULTIPLY_ALPHA);
    sprite->desc.ResetSize().ComputeCenter();
    sprite->renState = device->StateLoad(model->GetProjectPath().ResolvePath("default/rs/sprite_pma.ggrs"));

    return sprite;
}
