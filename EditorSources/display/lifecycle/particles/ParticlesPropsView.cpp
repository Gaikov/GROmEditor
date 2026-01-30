//
// Created by Roman on 12/15/2025.
//

#include "ParticlesPropsView.h"

#include "Engine/display/particles/VisualParticles.h"

bool nsParticlesPropsView::IsSupport(nsVisualObject2d *target) {
    return dynamic_cast<nsVisualParticles*>(target) != nullptr;
}

void nsParticlesPropsView::Draw(nsVisualObject2d *target) {
    const auto parts = dynamic_cast<nsVisualParticles*>(target);

    auto &system = parts->GetSystem();
    auto &b = system.behaviour;
    _particlesInput.Draw(b);

    _spawnInput.Draw(system.spawnEnabled);
    _preSpawnInput.Draw(parts->preSpawn);

    if (ImGui::Button("Remove Particles")) {
        system.RemoveAll();
    }

    _spaceInput.Draw(parts->space);
}
