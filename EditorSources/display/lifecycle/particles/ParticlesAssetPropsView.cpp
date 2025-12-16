//
// Created by Roman on 12/15/2025.
//

#include "ParticlesAssetPropsView.h"

#include "Engine/display/particles/VisualParticles.h"
#include "imgui/imgui.h"

bool nsParticlesAssetPropsView::IsSupport(nsVisualObject2d *target) {
    const auto ps = dynamic_cast<nsVisualParticles*>(target);
    return ps && ps->GetSystem().behaviour;
}

void nsParticlesAssetPropsView::Draw(nsVisualObject2d *target) {

    const auto ps = dynamic_cast<nsVisualParticles*>(target);
    const auto b = ps->GetSystem().behaviour;

    _particlesPerSec.Draw(b->amountPerSecond);
    _blastTime.Draw(b->spawnTime);

    if (ImGui::BeginTabBar("##parts_behaviour", ImGuiTabBarFlags_None)) {
        if (ImGui::BeginTabItem("Spawners")) {
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Updaters")) {
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Renderer")) {
            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }
}
