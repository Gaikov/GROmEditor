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
    if (!(b && b->spawnTime > 0)) {
        _spawnInput.Draw(system.spawnEnabled);
    }
    _spaceInput.Draw(parts->space);
}
