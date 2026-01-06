//
// Created by Roman on 1/6/2026.
//

#include "RendererPropsView.h"

#include "Core/undo/UndoService.h"
#include "Core/undo/UndoVarChange.h"
#include "Engine/renderer/particles/factory/ParticlesFactory.h"
#include "Engine/renderer/particles/renderer/ParticlesToonRenderer.h"
#include "Engine/renderer/particles/renderer/RoundParticlesRenderer.h"
#include "imgui/imgui.h"
#include "view/components/ColorInputUndo.h"
#include "view/components/FloatInputUndo.h"
#include "view/components/RenStateSelectUndo.h"
#include "view/components/TextureSelectUndo.h"

class nsRoundRendererPropsView final : public nsRendererPropsView {
public:
    bool IsSupported(nsParticlesRenderer *r) override {
        return dynamic_cast<nsParticlesRoundRenderer*>(r);
    }

    void Draw(nsParticlesRenderer *r, nsPropsContext *context) override {
        const auto s = dynamic_cast<nsParticlesRoundRenderer*>(r);

        _textureInput.Draw(s->texture);
        _renStateInput.Draw(s->renState);
    }

    nsTextureSelectUndo<ITexture*> _textureInput = "Texture";
    nsRenStateSelectUndo<IRenState*> _renStateInput;
};

class nsToonRendererPropsView final : public nsRendererPropsView {
public:
    bool IsSupported(nsParticlesRenderer *r) override {
        return dynamic_cast<nsParticlesToonRenderer*>(r);
    }

    void Draw(nsParticlesRenderer *r, nsPropsContext *context) override {
        const auto s = dynamic_cast<nsParticlesToonRenderer*>(r);

        _frontTexture.Draw(s->frontTexture);
        _frontColor.Draw(s->frontColor);
        _backTexture.Draw(s->backTexture);
        _backColor.Draw(s->backColor);
        _borderSize.Draw(s->borderSize);
        _renStateInput.Draw(s->renState);
    }

    nsTextureSelectUndo<ITexture*> _frontTexture = "Front Texture";
    nsColorInputUndo<nsColor> _frontColor = "Front Color";

    nsTextureSelectUndo<ITexture*> _backTexture = "Back Texture";
    nsColorInputUndo<nsColor> _backColor = "Back Color";

    nsFloatInputUndo<float> _borderSize = "Border Size";

    nsRenStateSelectUndo<IRenState*> _renStateInput;
};

nsRendererPropsRegistry::nsRendererPropsRegistry() {
    _factory.GetNamesList(_names);

    _views.emplace_back(new nsRoundRendererPropsView());
    _views.emplace_back(new nsToonRendererPropsView());
}

void nsRendererPropsRegistry::Draw(nsParticlesBehaviour *b) {
    auto current = "";
    if (b->renderer) {
        current = b->renderer->GetName();
    }

    if (ImGui::BeginCombo("Type", current)) {
        for (auto &name : _names) {
            const bool isSelected = name == current;
            if (ImGui::Selectable(name.c_str(), isSelected)) {
                const nsParticlesRenderer::sp_t r = _factory.Create(name.c_str());
                nsUndoService::Shared()->Push(new nsUndoVarChange(b->renderer, r));
            }

            if (isSelected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    DrawProps(b->renderer);
}
