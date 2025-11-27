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
    const auto undo = nsUndoService::Shared();

    _colorInput.Draw(sprite->desc.color);
    _textureInput.Draw(sprite->desc.tex);

    _sizeInput.Draw(sprite->desc.size);
    if (sprite->desc.tex) {
        ImGui::SameLine();
        if (ImGui::Button("Reset")) {
            int w, h;
            sprite->desc.tex->GetSize(w, h);
            undo->Push(new nsUndoVarChange(sprite->desc.size, nsVec2(w, h)));
        }
    }

    _pivotInput.Draw(sprite->desc.center);
    ImGui::SameLine();
    if (ImGui::Button("Center")) {
        const auto center = sprite->desc.size / 2.0f;
        undo->Push(new nsUndoVarChange(sprite->desc.center, center));
    }

    _uv1Input.Draw(sprite->desc.tex1);
    _uv2Input.Draw(sprite->desc.tex2);
}
