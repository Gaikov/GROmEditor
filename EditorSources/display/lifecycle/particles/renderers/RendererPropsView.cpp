//
// Created by Roman on 1/6/2026.
//

#include "RendererPropsView.h"

#include "Core/undo/UndoService.h"
#include "Core/undo/UndoVarChange.h"
#include "Engine/renderer/particles/factory/ParticlesFactory.h"
#include "imgui/imgui.h"

nsRendererPropsRegistry::nsRendererPropsRegistry() {
    _factory.GetNamesList(_names);
}

void nsRendererPropsRegistry::DrawProps(nsParticlesBehaviour *b) {
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

    for (const auto &prop : m_props) {
        if (prop->IsSupported(b->renderer)) {
            prop->Draw(b->renderer);
        }
    }
}
