//
// Created by Roman on 11/25/2025.
//

#include "SpritePropsView.h"
#include "Core/undo/UndoBatch.h"
#include "Engine/RenManager.h"
#include "Engine/display/sprite/Sprite.h"
#include <cstring>

bool nsSpritePropsView::IsSupport(nsVisualObject2d *target) {
    return dynamic_cast<nsSprite*>(target) != nullptr;
}

void nsSpritePropsView::Draw(nsVisualObject2d *target) {
    const auto sprite = dynamic_cast<nsSprite*>(target);
    const auto undo = nsUndoService::Shared();
    const auto device = nsRenDevice::Shared()->Device();

    _textureInput.SetLoadFlags(sprite->desc.premultiplyAlpha ? TLF_PREMULTIPLY_ALPHA : TLF_NONE);
    _textureInput.Draw(sprite->desc.tex);

    bool premultiplyAlpha = sprite->desc.premultiplyAlpha;
    if (ImGui::Checkbox("Premultiply Alpha", &premultiplyAlpha)) {
        const nsFilePath pmaStatePath = _model->GetProjectPath().ResolvePath("default/rs/sprite_pma.ggrs");
        const nsFilePath defaultStatePath = _model->GetProjectPath().ResolvePath("default/rs/gui.ggrs");
        const int flags = premultiplyAlpha ? TLF_PREMULTIPLY_ALPHA : TLF_NONE;

        ITexture *texture = sprite->desc.tex;
        if (texture) {
            const nsString texturePath = device->TextureGetPath(texture);
            texture = texturePath.IsEmpty()
                      ? nullptr
                      : device->TextureLoad(texturePath, false, TF_RGBA, flags);
        }

        IRenState *state = sprite->renState;
        if (premultiplyAlpha) {
            state = device->StateLoad(pmaStatePath);
        } else if (state && device->StateGetPath(state)
                   && strcmp(device->StateGetPath(state), pmaStatePath.AsChar()) == 0) {
            state = device->StateLoad(defaultStatePath);
        }

        const auto batch = new nsUndoBatch();
        batch->Add(new nsUndoVarChange(sprite->desc.premultiplyAlpha, premultiplyAlpha));
        if (texture != sprite->desc.tex) {
            batch->Add(new nsUndoVarChange(sprite->desc.tex, texture));
        }
        if (state != sprite->renState) {
            batch->Add(new nsUndoVarChange(sprite->renState, state));
        }
        undo->Push(batch);
    }

    _shaderInput.Draw(sprite->renState);
    _colorInput.Draw(sprite->desc.color);

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
    _frameInput.Draw(sprite->drawFrame);
}
