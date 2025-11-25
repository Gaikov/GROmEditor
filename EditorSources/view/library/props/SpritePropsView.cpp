//
// Created by Roman on 11/25/2025.
//

#include "SpritePropsView.h"

#include "Engine/display/sprite/Sprite.h"

bool nsSpritePropsView::IsSupport(nsVisualObject2d *target) {
    return dynamic_cast<nsSprite*>(target) != nullptr;
}

void nsSpritePropsView::Draw(nsVisualObject2d *target) {
    const auto sprite = dynamic_cast<nsSprite*>(target);

    _colorInput.Draw(sprite->desc.color);
}
